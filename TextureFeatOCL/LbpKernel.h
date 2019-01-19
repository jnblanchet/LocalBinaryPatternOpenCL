#pragma once
#include "SimpleKernel.h"
class LbpKernel :
	public SimpleKernel
{
public:
	LbpKernel(char* kernelFileName, char* kernelFunctionName) : SimpleKernel(kernelFileName, kernelFunctionName) {};
	unsigned int* extractLBP(size_t size, unsigned char* image);
	~LbpKernel();
};

