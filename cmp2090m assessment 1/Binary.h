#ifndef Binary_H
#define Binary_H

#include "Matrix.h";

class Binary : public Matrix
{
public:
	Binary();
	Binary(Matrix& _aMatrix, int threshold);
	~Binary();
};
#endif