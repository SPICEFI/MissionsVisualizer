#include "Matrix3x3.h"

const Matrix3x3 Matrix3x3::Identity = Matrix3x3();

Matrix3x3::Matrix3x3()
{
	for(int i = 0; i < 3; i++)
	{
		for(int j = 0; j < 3; j++)
		{
			if(i == j)
				matrix[i][j] = 1.0f;
			else
				matrix[i][j] = 0.0f;
		}
	}
}

Matrix3x3::Matrix3x3(const float* m)
{
	Set(m);
}

Matrix3x3::Matrix3x3(const float m[3][3])
{
	Set(&m[0][0]);
}

void Matrix3x3::Set(const float* m)
{
	std::memcpy(matrix, m, 3 * 3 * sizeof(m));
}

void Matrix3x3::Set(size_t row, size_t col, float val)
{
	matrix[row][col] = val;
}

const float* Matrix3x3::Get() const
{
	return &matrix[0][0];
}

float Matrix3x3::Get(size_t row, size_t col) const
{
	return matrix[row][col];
}
