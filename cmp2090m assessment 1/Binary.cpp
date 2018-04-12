#ifndef Binary_CPP
#define Binary_CPP

#include "Matrix.h"
#include "Binary.h"



Binary::Binary(Matrix& existing, int threshold)
{
	imageSizeC = existing.getSizeC();
	imageSizeR = existing.getSizeR();

	imageArray = new double[imageSizeC * imageSizeR];

	for (int x = 0; x < (imageSizeC * imageSizeR); x++)
	{
		if (existing.getPixel(x) < threshold)
			imageArray[x] = 0;
		else
			imageArray[x] = 1;
	}
}

Binary::~Binary()
{
}
#endif
