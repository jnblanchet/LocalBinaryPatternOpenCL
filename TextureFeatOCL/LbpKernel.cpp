#include "LbpKernel.h"

#define MEM_SIZE (256)

unsigned int* LbpKernel::extractLBP(size_t size, unsigned char* image)
{
	cl_int ret;

	// Create memory buffer on the GPU for return value
	cl_mem gpuReturnBuffer = clCreateBuffer(this->context, CL_MEM_READ_WRITE, MEM_SIZE * sizeof(unsigned int), NULL, &ret);
	displayErrorCode(ret, "extractLBP - creating return buffer on GPU");

	cl_mem gpuInputImageBuffer = clCreateBuffer(this->context,CL_MEM_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(unsigned char) * size*size, image, &ret);
	displayErrorCode(ret, "extractLBP - creating input image buffer on GPU");

	// Set OpenCL kernel parameters
	ret = clSetKernelArg(this->kernel, 0, sizeof(cl_mem), (void *)&gpuInputImageBuffer); // const __global unsigned char* image
	displayErrorCode(ret, "extractLBP - setting first kernel argument");

	ret = clSetKernelArg(this->kernel,1,sizeof(cl_mem), (void *)&gpuReturnBuffer); // __global int* lbpHist
	displayErrorCode(ret, "extractLBP - setting second kernel argument");

	// Execute OpenCL kernel
	cl_event event;
	size_t globalNDWorkSize[2] = { size , size};
	size_t localNDWorkSize[2] = { 32 , 32 };
	ret = clEnqueueNDRangeKernel(		this->command_queue, 		// command_queue pointer
										this->kernel,				// kernel pointer
										2, 							// number of dimensions: 1,2 or 3 (data-specific)
										NULL, 						// global_work_offset: starting index for each point
										globalNDWorkSize, 			// global_work_size: global work-items *array[dim]
										localNDWorkSize,			// local_work_size: local work-items *array[dim]
										0,							// num_events_in_wait_list
										NULL,						// *event_wait_list,
										&event);					// *event (returned)

	displayErrorCode(ret, "extractLBP - executing kernel");

	//ret = clEnqueueTask(this->command_queue, this->kernel, 0, NULL, NULL);
	//displayErrorCode(ret, "extractLBP - description here");
	//clEnqueueTask = task-parallel instructions (many different functions on same or different data)
	//clEnqueueNDRangeKernel = data-parallel instructions (single function, many different data)

	// Copy results from the memory buffer
	unsigned int* lbpHist = new unsigned int[256];
	ret = clEnqueueReadBuffer(this->command_queue, gpuReturnBuffer, CL_TRUE, 0,
		MEM_SIZE * sizeof(unsigned int), lbpHist, 0, NULL, NULL);
	displayErrorCode(ret, "extractLBP - reading result sychronously");

	clReleaseMemObject(gpuReturnBuffer);
	clReleaseMemObject(gpuInputImageBuffer);

	return lbpHist;
}


LbpKernel::~LbpKernel()
{

}
