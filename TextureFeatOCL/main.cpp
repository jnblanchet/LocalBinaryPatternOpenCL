#include <iostream>
#include <ctime>
#include <iomanip>
#include "LbpKernel.h"
#include "Timer.h"

using namespace std;

char LUT[9] = {'.',':','-','=','+','*','#','%','@'}; //for ascii art display

// Image creation helper.
// TODO: read the image from the hard drive instead.
void createRandomImage(int size, unsigned char** outImage) {
	(*outImage) = new unsigned char[size * size];

	srand(time(NULL));

	for (int i = 0; i < size*size; i++) {
		(*outImage)[i] = rand() % 256;
	}

}

unsigned int* extractLbpCpu(unsigned char* image, int imageW)
{
	unsigned int* lbpHist = new unsigned int[256];
	for (int i = 0; i < 256 ; i++)
		lbpHist[i] = 0;
	const int bitOrder[8] = { 0,1,2,7,3,6,5,4 }; // circular clock-wise LBP ordering

	for (int y = 1; y < imageW - 1; y++) {
		for (int x = 1; x < imageW - 1; x++) {
			unsigned char refVal = image[x + y*imageW];
			unsigned int code = 0;
			int bitcounter = 0;
			for (int offY = -1; offY <= 1; offY++) {
				const int idxRowOffset = (y + offY) * imageW;
				for (int offX = -1; offX<=1; offX++) {
					if (offX != 0 || offY != 0) {
						if (image[x + offX + idxRowOffset] >= refVal) {
							code = code | (0x1 << bitOrder[bitcounter]);
						}
						bitcounter++;
					}
				}
			}
			lbpHist[code]++;
		}
	}
	return lbpHist;
}

int main()
{
	cout << "This program will measure the time required to extract Local Binary Pattern (LBP) on a GPU..." << endl;
	const int s1 = 5, s2 = 12; // will generate random images square with sizes from 2^s1 to 2^s2
	const int minSize = 0x1 << s1;
	const int maxSize = 0x1 << s2;
	double gpuTimes[s2-s1+1];
	double cpuTimes[s2-s1+1];
	CPerfCounter counter;
	int inter = -1;


	cout << "Creating LBP Kernel..." << endl;
	LbpKernel kLBP = LbpKernel("./LBP.cl", "LBP");

	for (int size = minSize; size <= maxSize; size <<= 1) {
		cout << endl << "Creating image of size " << size << " by " << size << "..." << endl;
		unsigned char* image = NULL;
		createRandomImage(size, &image);

		//GPU version
		cout << endl << "Launching LBP kernel (GPU)..." << endl;
		counter.Reset();
		counter.Start();
		unsigned int* lbpHistGPU = kLBP.extractLBP(size, image);
		counter.Stop();
		gpuTimes[++inter] = counter.GetElapsedTime();

		//CPU version
		cout << endl << "Launching LBP CPU version..." << endl;
		counter.Reset();
		counter.Start();
		unsigned int* lbpHistCPU = extractLbpCpu(image, size);
		counter.Stop();
		cpuTimes[inter] = counter.GetElapsedTime();

		cout << endl << "R E S U L T S" << endl << "=============" << endl;
		/*cout << endl << "input image:" << endl;
		for (int y = 0; y < size; y++) {
			for (int x = 0; x < size; x++) {
				//cout << setw(4) << (int)(image[y * size + x]); // gray levels mode
				cout << LUT[image[y * size + x] / 25]; // ascii art mode
			}
			cout << endl;
		}*/

		cout << endl << "lbpHistogram:" << endl;
		cout << setw(15) << "lbp code" << setw(15) << "GPU LBP hist" << setw(15) << "CPU LBP hist" << endl;
		for (int i = 0; i < 256; i++) {
			if(lbpHistGPU[i] != 0)
				cout << setw(15) << i << setw(15) << lbpHistGPU[i] << setw(15) << lbpHistCPU[i] << endl;
		}

		cout << endl << "Cleaning up RAM..." << endl;
		delete image;
		delete lbpHistCPU;
		delete lbpHistGPU;
	}

	cout << endl << "Execution time report..." << endl;
	for (int i = 0; i <= inter; i++) {
		cout << " Image size " << (0x1 << (s1 + i)) << " took " << gpuTimes[i] << " on GPU, and " << cpuTimes[i] << " on CPU." << endl;
	}

	system("pause");
	return 0;
}
