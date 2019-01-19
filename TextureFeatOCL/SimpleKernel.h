#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <CL/opencl.h>

class SimpleKernel
{
protected:
	cl_device_id device_id = NULL;
	cl_context context = NULL;
	cl_command_queue command_queue = NULL;
	//cl_mem memobj = NULL;
	cl_program program = NULL;
	cl_kernel kernel = NULL;
	cl_platform_id platform_id = NULL;
	
	void displayErrorCode(int code, char * description);
	char* codeToString(int code);
public:
	SimpleKernel(char* kernelFileName, char* kernelFunctionName);
	virtual ~SimpleKernel();

};

