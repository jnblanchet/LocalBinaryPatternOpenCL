#include "SimpleKernel.h"
#include <iostream>

#define MAX_SOURCE_SIZE (0x100000)
/*protected return buffer*/

SimpleKernel::SimpleKernel(char* kernelFileName, char* kernelFunctionName)
{
	cl_int ret;
	cl_uint ret_num_devices;
	cl_uint ret_num_platforms;

	FILE *fp;
	char *source_str;
	size_t source_size;

	// Load the source code containing the kernel
	fopen_s(&fp, kernelFileName, "r");
	if (!fp) {
		fprintf(stderr, "Failed to load kernel.\n");
		exit(1);
	}
	source_str = (char*)malloc(MAX_SOURCE_SIZE);
	source_size = fread(source_str, 1, MAX_SOURCE_SIZE, fp);
	fclose(fp);

	// Get platform and device info 
	ret = clGetPlatformIDs(1, &this->platform_id, &ret_num_platforms);
	displayErrorCode(ret, "SimpleKernel constructor - getting platform ids");
	ret = clGetDeviceIDs(this->platform_id, CL_DEVICE_TYPE_GPU, 1, &this->device_id, &ret_num_devices); // CL_DEVICE_TYPE_DEFAULT, CL_DEVICE_TYPE_GPU or CL_DEVICE_TYPE_CPU
	displayErrorCode(ret, "SimpleKernel constructor - getting device ID");

	// Create OpenCL context 
	this->context = clCreateContext(NULL, 1, &this->device_id, NULL, NULL, &ret);
	displayErrorCode(ret, "SimpleKernel constructor - creating context");

	// Create command queue
	this->command_queue = clCreateCommandQueue(this->context, this->device_id, 0, &ret);
	displayErrorCode(ret, "SimpleKernel constructor - creating command queue");

	// Create kernel program from the source
	this->program = clCreateProgramWithSource(this->context, 1, (const char **)&source_str,
		(const size_t *)&source_size, &ret); // clCreateProgramWithBinary if from binary
	displayErrorCode(ret, "SimpleKernel constructor - creating program from source");

	// Build kernel program
	ret = clBuildProgram(this->program, 1, &this->device_id, NULL, NULL, NULL);
	displayErrorCode(ret, "SimpleKernel constructor - building kernel program");

	// Create OpenCL kernel
	this->kernel = clCreateKernel(this->program, kernelFunctionName, &ret);
	displayErrorCode(ret, "SimpleKernel constructor - creating kernel from program");

	// Release kernel source code memory
	free(source_str);
}


SimpleKernel::~SimpleKernel()
{
	cl_int ret;
	ret = clFlush(this->command_queue);
	ret = clFinish(this->command_queue);
	ret = clReleaseKernel(this->kernel);
	ret = clReleaseProgram(this->program);
	ret = clReleaseCommandQueue(this->command_queue);
	ret = clReleaseContext(this->context);
}

void SimpleKernel::displayErrorCode(int code, char* description)
{
	if(code != CL_SUCCESS)
		std::cout << "OpenCL error code " << codeToString(code) << " at step: " << description << "!" << std::endl;
}

char* SimpleKernel::codeToString(int code) {
	char * strCode = "";
		if (code == CL_SUCCESS)
			strCode = "CL_SUCCESS\0";
		else if (code == CL_DEVICE_NOT_FOUND)
			strCode = "CL_DEVICE_NOT_FOUND\0";
		else if (code == CL_DEVICE_NOT_AVAILABLE)
			strCode = "CL_DEVICE_NOT_AVAILABLE\0";
		else if (code == CL_COMPILER_NOT_AVAILABLE)
			strCode = "CL_COMPILER_NOT_AVAILABLE\0";
		else if (code == CL_MEM_OBJECT_ALLOCATION_FAILURE)
			strCode = "CL_MEM_OBJECT_ALLOCATION_FAILURE\0";
		else if (code == CL_OUT_OF_RESOURCES)
			strCode = "CL_OUT_OF_RESOURCES\0";
		else if (code == CL_OUT_OF_HOST_MEMORY)
			strCode = "CL_OUT_OF_HOST_MEMORY\0";
		else if (code == CL_PROFILING_INFO_NOT_AVAILABLE)
			strCode = "CL_PROFILING_INFO_NOT_AVAILABLE\0";
		else if (code == CL_MEM_COPY_OVERLAP)
			strCode = "CL_MEM_COPY_OVERLAP\0";
		else if (code == CL_IMAGE_FORMAT_MISMATCH)
			strCode = "CL_IMAGE_FORMAT_MISMATCH\0";
		else if (code == CL_IMAGE_FORMAT_NOT_SUPPORTED)
			strCode = "CL_IMAGE_FORMAT_NOT_SUPPORTED\0";
		else if (code == CL_BUILD_PROGRAM_FAILURE)
			strCode = "CL_BUILD_PROGRAM_FAILURE\0";
		else if (code == CL_MAP_FAILURE)
			strCode = "CL_MAP_FAILURE\0";
		else if (code == CL_INVALID_VALUE)
			strCode = "CL_INVALID_VALUE\0";
		else if (code == CL_INVALID_DEVICE_TYPE)
			strCode = "CL_INVALID_DEVICE_TYPE\0";
		else if (code == CL_INVALID_PLATFORM)
			strCode = "CL_INVALID_PLATFORM\0";
		else if (code == CL_INVALID_DEVICE)
			strCode = "CL_INVALID_DEVICE\0";
		else if (code == CL_INVALID_CONTEXT)
			strCode = "CL_INVALID_CONTEXT\0";
		else if (code == CL_INVALID_QUEUE_PROPERTIES)
			strCode = "CL_INVALID_QUEUE_PROPERTIES\0";
		else if (code == CL_INVALID_COMMAND_QUEUE)
			strCode = "CL_INVALID_COMMAND_QUEUE\0";
		else if (code == CL_INVALID_HOST_PTR)
			strCode = "CL_INVALID_HOST_PTR\0";
		else if (code == CL_INVALID_MEM_OBJECT)
			strCode = "CL_INVALID_MEM_OBJECT\0";
		else if (code == CL_INVALID_IMAGE_FORMAT_DESCRIPTOR)
			strCode = "CL_INVALID_IMAGE_FORMAT_DESCRIPTOR\0";
		else if (code == CL_INVALID_IMAGE_SIZE)
			strCode = "CL_INVALID_IMAGE_SIZE\0";
		else if (code == CL_INVALID_SAMPLER)
			strCode = "CL_INVALID_SAMPLER\0";
		else if (code == CL_INVALID_BINARY)
			strCode = "CL_INVALID_BINARY\0";
		else if (code == CL_INVALID_BUILD_OPTIONS)
			strCode = "CL_INVALID_BUILD_OPTIONS\0";
		else if (code == CL_INVALID_PROGRAM)
			strCode = "CL_INVALID_PROGRAM\0";
		else if (code == CL_INVALID_PROGRAM_EXECUTABLE)
			strCode = "CL_INVALID_PROGRAM_EXECUTABLE\0";
		else if (code == CL_INVALID_KERNEL_NAME)
			strCode = "CL_INVALID_KERNEL_NAME\0";
		else if (code == CL_INVALID_KERNEL_DEFINITION)
			strCode = "CL_INVALID_KERNEL_DEFINITION\0";
		else if (code == CL_INVALID_KERNEL)
			strCode = "CL_INVALID_KERNEL\0";
		else if (code == CL_INVALID_ARG_INDEX)
			strCode = "CL_INVALID_ARG_INDEX\0";
		else if (code == CL_INVALID_ARG_VALUE)
			strCode = "CL_INVALID_ARG_VALUE\0";
		else if (code == CL_INVALID_ARG_SIZE)
			strCode = "CL_INVALID_ARG_SIZE\0";
		else if (code == CL_INVALID_KERNEL_ARGS)
			strCode = "CL_INVALID_KERNEL_ARGS\0";
		else if (code == CL_INVALID_WORK_DIMENSION)
			strCode = "CL_INVALID_WORK_DIMENSION\0";
		else if (code == CL_INVALID_WORK_GROUP_SIZE)
			strCode = "CL_INVALID_WORK_GROUP_SIZE\0";
		else if (code == CL_INVALID_WORK_ITEM_SIZE)
			strCode = "CL_INVALID_WORK_ITEM_SIZE\0";
		else if (code == CL_INVALID_GLOBAL_OFFSET)
			strCode = "CL_INVALID_GLOBAL_OFFSET\0";
		else if (code == CL_INVALID_EVENT_WAIT_LIST)
			strCode = "CL_INVALID_EVENT_WAIT_LIST\0";
		else if (code == CL_INVALID_EVENT)
			strCode = "CL_INVALID_EVENT\0";
		else if (code == CL_INVALID_OPERATION)
			strCode = "CL_INVALID_OPERATION\0";
		else if (code == CL_INVALID_GL_OBJECT)
			strCode = "CL_INVALID_GL_OBJECT\0";
		else if (code == CL_INVALID_BUFFER_SIZE)
			strCode = "CL_INVALID_BUFFER_SIZE\0";
		else if (code == CL_INVALID_MIP_LEVEL)
			strCode = "CL_INVALID_MIP_LEVEL\0";
		else if (code == CL_INVALID_GLOBAL_WORK_SIZE)
			strCode = "CL_INVALID_GLOBAL_WORK_SIZE\0";
		return strCode;
}