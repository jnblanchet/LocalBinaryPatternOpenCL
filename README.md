## LocalBinaryPatternOpenCL

A minimalistic LBP (8 neighbors, 255 bins) GPU implementation in OpenCL (C++) for the purpose of learning OpenCL.

Tested on NVDIAI GTX 960 and Intel i7 920 (2.67 Ghz)

	Resolution (px)		cpu (ms)		gpu (ms)
	2048 x 2048 		16.7			6.3
	4096 x 4096 		65.9			18.2
	8192x 8192		2632			63.8
	16384 x 16384 		10564			232.3
