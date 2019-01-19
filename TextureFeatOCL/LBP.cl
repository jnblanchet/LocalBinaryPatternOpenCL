//LBP extraction opencl gpu kernel (lbp algorithm with radius=1 and sampling = 8)
// input: an image
// output: a histogram with 256 classes (256 unique lbp codes)

__kernel void LBP(const __global  uchar* image,	// pointer to the input image (byte array)
                    __global uint* lbpHist)		// pointer to the resulting lbp histogram
{
	const int imageW = get_global_size(0); // the width of the input image

    const int x = get_global_id(0); // x coord of pixel
    const int y = get_global_id(1); // y coord of pixel
	
	if (x > 0 && x < imageW-1 && y > 0 && y < imageW-1) {
		__private const int bitOrder[8] = {0,1,2,7,3,6,5,4}; // circular clock-wise LBP ordering

		__private uchar refVal = image[x+y*imageW];
		__private uint code = 0;
		__private int bitcounter = 0;
		for (int offY=-1; offY<=1; offY++) {
			const int idxRowOffset = (y+offY) * imageW;
			for (int offX=-1; offX<=1; offX++) {
				if(offX != 0 || offY != 0) {
					if(image[x+offX+idxRowOffset] >= refVal) {
						code = code | (1 << bitOrder[bitcounter]);
					}
					bitcounter++;
				}
			}
		}
		//lbpHist[code]++; // This is not atomic.
		atomic_add(&lbpHist[code] , 1); // an efficient alternative would be a tree structured reduce.
	}
}
