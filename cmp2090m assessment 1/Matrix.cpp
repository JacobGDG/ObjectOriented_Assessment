#ifndef Matrix_CPP
#define Matrix_CPP

#include <iostream>
#include <fstream>
#include <istream>
#include <sstream>

using namespace std;

#include "Matrix.h"


	//contructors
	Matrix::Matrix()
	{
		imageSizeC = 0;
		imageSizeR = 0;

		imageArray = NULL;

		//cout << "Matrix::Matrix() is invoked\n";
	}
	Matrix::Matrix(int M, int N, char* fileName)
	{
		imageSizeR = M;//number of pixels tall
		imageSizeC = N;//number of pixels wide
		double* _temp = readTXT(fileName);
		imageArray = new double[N * M];//create new array of correct size in pointer location

		for (int x = 0; x < (N * M); x++)//assign val to every postion in array as place holder
			imageArray[x] = _temp[x];

		delete [] _temp;
		cout << "Matrix::Matrix(int N, int M, char* fileName) is invoked\n";
	}
	Matrix::Matrix(int M, int N, double* image)
	{
		imageSizeR = M;//number of pixels tall
		imageSizeC = N;//number of pixels wide
		imageArray = new double[N * M];//create new array of correct size in pointer location
		
		for (long x = 0; x < (N * M); x++)//loop through image and assign each value to same location in mainImage
			imageArray[x] = image[x];

		delete image;
		cout << "Matrix::Matrix(int N, int M, double* image) is invoked\n";
	}
	Matrix::Matrix(int M, int N, char* fileName, double threshold, int white, int black)//white is either 1 or 255 depending on use 1 = wally comparison 255 = logo with noise
	{
		imageSizeR = M;//number of pixels tall
		imageSizeC = N;//number of pixels wide
		double* _temp = readTXT(fileName);
		imageArray = new double[N * M];//create new array of correct size in pointer location

		for (int x = 0; x < (N * M); x++)//assign val to every postion in array as place holder
			imageArray[x] = _temp[x];

		delete[] _temp;

		for (long x = 0; x < (imageSizeR * imageSizeC); x++)
		{
			if (imageArray[x] < threshold)
				imageArray[x] = black;
			else
				imageArray[x] = white;
		}
		cout << "Matrix::Matrix(int M, int N, char* fileName, double threshold, int white) is invoked\n";
	}
	Matrix::Matrix(const Matrix& original, int startN, int startM, int endN, int endM)//this will be used when selecting part of the bigger image
	{
		imageSizeR = endM - startM + 1;//difference between M min and max
		imageSizeC = endN - startN + 1;//difference between N min and max
		imageArray = new double[imageSizeC * imageSizeR];//create array same size that 2D array would be

		int x = 0;//counter
		for (int m = startM; m <= endM; m++)//loop through all rows within startM endM range
			for (int n = startN; n <= endN; n++)//loop through all columns within startN endN range
			{
				imageArray[x] = original.imageArray[(m*original.imageSizeC) + n];//store values in new array, have to convert matrix positions to array position using ConvertIJ
				x++;//counter increment
			}
		//cout << "Matrix::Matrix(const Matrix& original, int startM, int startN, int endM, int endN) is invoked\n";
	}

	//copy constructor
	Matrix::Matrix(const Matrix& original)
	{
		imageSizeC = original.imageSizeC;
		imageSizeR = original.imageSizeR;

		imageArray = new double [imageSizeC * imageSizeR];

		for (long x = 0; x < (imageSizeC * imageSizeR); x++)//loop through image and assign each value to same location in mainImage
			imageArray[x] = original.imageArray[x];

		//cout << "Matrix::Matrix(const Matrix& original)\n";
	}
	void Matrix::operator=(const Matrix& original)
	{
		imageSizeC = original.imageSizeC;
		imageSizeR = original.imageSizeR;

		imageArray = new double[imageSizeC * imageSizeR];

		for (long x = 0; x < (imageSizeC * imageSizeR); x++)//loop through image and assign each value to same location in mainImage
			imageArray[x] = original.imageArray[x];

		//cout << "Matrix Matrix::operator= (const Matrix& original)\n";
	}

	//destructors
	Matrix::~Matrix()//called whenever object goes out of scope or program closes
	{
		delete [] imageArray;//delete pointer array imageArray
		//cout << "Matrix::~Matrix() is invoked\n";
	}

	//functions
	void Matrix::displayMatrix()
	{
		for (int x = 0; x < (imageSizeR*imageSizeC); x++)
		{
			cout << imageArray[x] << "\t";
			if (((x + 1) % imageSizeC) == 0)
				cout << endl;
		}
		cout<<endl;
		
	}
	double* Matrix::readTXT(char* fileName)
	{
		double* data = new double[imageSizeR*imageSizeC];
		int i = 0;
		ifstream myfile(fileName);
		if (myfile.is_open())
		{

			while (myfile.good())
			{
				if (i>imageSizeR*imageSizeC - 1) break;
				myfile >> *(data + i);
				//cout << *(data+i) << ' '; // This line display the converted data on the screen, you may comment it out. 
				i++;
			}
			myfile.close();
		}

		else cout << "Unable to open file";
		cout << i;

		return data;
	}
	void Matrix::WritePGM(char *filename, int Q)
	{
		int i;
		unsigned char *image;
		ofstream myfile;

		image = (unsigned char *) new unsigned char[imageSizeR*imageSizeC];

		// convert the integer values to unsigned char

		for (i = 0; i<imageSizeR*imageSizeC; i++)
			image[i] = (unsigned char)imageArray[i];

		myfile.open(filename, ios::out | ios::binary | ios::trunc);

		if (!myfile) {
			cout << "Can't open file: " << filename << endl;
			exit(1);
		}

		myfile << "P5" << endl;
		myfile << imageSizeC << " " << imageSizeR << endl;
		myfile << Q << endl;

		myfile.write(reinterpret_cast<char *>(image), (imageSizeR*imageSizeC)*sizeof(unsigned char));

		if (myfile.fail()) {
			cout << "Can't write image " << filename << endl;
			exit(0);
		}

		myfile.close();

		delete[] image;
		cout << "void Matrix::WritePGM(char *filename, int Q) is invoked\n";
	}
	void Matrix::whiteOutSegment(const Matrix& newSegment, int m, int n)
	{
		for (int M = m; M < (m + newSegment.imageSizeR); M++)
			for (int N = n; N < (n + newSegment.imageSizeC); N++)
			{
				imageArray[(M*imageSizeC) + N] = 255;
			}
		cout << "void Matrix::whiteOutSegment(int m, int n, const Matrix& newSegment) is invoked\n";
	}
	void Matrix::switchSegment(int m1, int n1, int m2, int n2, int sizeR, int sizeC)
	{
		double* tempImageArray1 = new double[sizeR * sizeC];
		double* tempImageArray2 = new double[sizeR * sizeC];

		int count = 0;
		for (int M = m1; M < (m1 + sizeR); M++)
			for (int N = n1; N < (n1 + sizeC); N++)
			{
				tempImageArray1[count] = imageArray[(M*imageSizeC) + N];
				count++;
			}

		count = 0;
		for (int M = m2; M < (m2 + sizeR); M++)
			for (int N = n2; N < (n2 + sizeC); N++)
			{
				tempImageArray2[count] = imageArray[(M*imageSizeC) + N];
				count++;
			}

		count = 0;
		for (int M = m1; M < (m1 + sizeR); M++)
			for (int N = n1; N < (n1 + sizeC); N++)
			{
				imageArray[(M*imageSizeC) + N] = tempImageArray2[count];
				count++;
			}

		count = 0;
		for (int M = m2; M < (m2 + sizeR); M++)
			for (int N = n2; N < (n2 + sizeC); N++)
			{
				imageArray[(M*imageSizeC) + N] = tempImageArray1[count];
				count++;
			}

		delete[] tempImageArray1;
		delete[] tempImageArray2;
		//cout << "void Matrix::replaceSegment(int m, int n, const Matrix& newSegment) is invoked\n";
	}
	
	double Matrix::CalculateVariance()
	{
		Square();
		return SumOf();
	}

	double Matrix::findMean()
	{
		double mean = SumOf() / (imageSizeC*imageSizeR);

		return mean;
	}
	void Matrix::Square()
	{
		for (int x = 0; x < (imageSizeC * imageSizeR); x++)
			imageArray[x] = pow(imageArray[x], 2);

	}
	double Matrix::SumOf()
	{
		double Sum = 0;
		for (int x = 0; x < (imageSizeR * imageSizeC); x++)
			Sum += imageArray[x];
	
		return Sum;
	}

	Matrix Matrix::operator*(const Matrix& existing)
	{
		Matrix temp;

		temp.imageSizeR = existing.imageSizeR;
		temp.imageSizeC = existing.imageSizeC;

		temp.imageArray = new double[temp.imageSizeR * temp.imageSizeC];

		for (int x = 0; x < (temp.imageSizeR * temp.imageSizeC); x++)
		{
			temp.imageArray[x] = imageArray[x] * existing.imageArray[x];
		}
		//cout << "Matrix Matrix::operator*(const Matrix& existing) is invoked\n";
		return temp;
	}
	double Matrix::operator==(const Matrix& existing)
	{
		bool match = true;
		for (int x = 0; x < (imageSizeR * imageSizeC); x++)
		{
			
			if (imageArray[x] != existing.imageArray[x])
			{
				cout << x << ": " << imageArray[x] << " == " << existing.imageArray[x] << endl;
				match = false;
				//break;
			}
		}
		//cout << "bool Matrix::operator==(const Matrix& existing) is invoked\n";
		return match;
	}
	Matrix Matrix::operator-(const Matrix& existing)
	{
		Matrix temp;

		temp.imageSizeR = existing.imageSizeR;
		temp.imageSizeC = existing.imageSizeC;

		temp.imageArray = new double[temp.imageSizeR * temp.imageSizeC];

		for (int x = 0; x < (temp.imageSizeR * temp.imageSizeC); x++)
			temp.imageArray[x] = imageArray[x] - existing.imageArray[x];
		//cout << "Matrix Matrix::operator-(const Matrix& existing) is invoked\n";
		return temp;
	}
	Matrix Matrix::operator-(double mean)
	{
		Matrix temp;

		temp.imageSizeR = imageSizeR;
		temp.imageSizeC = imageSizeC;

		temp.imageArray = new double[temp.imageSizeR * temp.imageSizeC];

		for (int x = 0; x < (imageSizeR * imageSizeC); x++)
			temp.imageArray[x] -= mean;

		return temp;
	}

	//authorization
	int Matrix::getSizeC()
	{
		return imageSizeC;
	}
	int Matrix::getSizeR()
	{

		return imageSizeR;
	}
	double Matrix::getPixel(int num)
	{
		return imageArray[num];
	}
	double Matrix::getPixel(int M, int N)
	{
		return imageArray[(M*imageSizeC) + N];
	}
	;
#endif