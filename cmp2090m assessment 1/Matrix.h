#ifndef Matrix_H
#define Matrix_H

class Matrix
{
protected:
	double* imageArray;//image matrix will be stored here
	int imageSizeC, imageSizeR;
	
	Matrix();	
	;
public:
	

	Matrix(int M, int N);
	Matrix(int M, int N, char* fileName);
	Matrix(int M, int N, double* image);
	Matrix(int M, int N, char* fileName, double threshold, int white, int black);
	Matrix(const Matrix& original, int startN, int startM, int endN, int endM);
	
	Matrix(const Matrix& original);
	void operator= (const Matrix& original);

	~Matrix();

	void displayMatrix();
	double* readTXT(char* fileName);
	void WritePGM(char *filename, int Q);
	void whiteOutSegment(const Matrix& newSegment, int m, int n);
	void switchSegment(int m1, int n1, int m2, int n2, int sizeR, int sizeC);

	double CalculateVariance();
	double CalculateNC();

	double Matrix::findMean();
	void Square();
	double SumOf();

	Matrix operator* (const Matrix& existing);
	double operator== (const Matrix& existing);
	Matrix operator- (const Matrix& existing);
	Matrix Matrix::operator-(double);

	int getSizeC();
	int getSizeR();
	double getPixel(int num);
	double getPixel(int m, int n);
	;
};

#endif