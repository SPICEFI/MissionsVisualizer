#include "Matrix4x4.h"

const Matrix4x4 Matrix4x4::Identity = Matrix4x4(1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f);
const Matrix4x4 Matrix4x4::Zero = Matrix4x4();

Matrix4x4::Matrix4x4()
{
	for(int i = 0; i < DIM; i++)
	{
		for(int j = 0; j < DIM; j++)
		{
			matrix[i][j] = 0.0f;
		}
	}
}

Matrix4x4::Matrix4x4(const float* m)
{
	Set(m);
}

Matrix4x4::Matrix4x4(const float m[DIM][DIM])
{
	Set(&m[0][0]);
}

Matrix4x4::Matrix4x4(float m11, float m12, float m13, float m14,
					 float m21, float m22, float m23, float m24,
					 float m31, float m32, float m33, float m34,
					 float m41, float m42, float m43, float m44)
{
	const float m[DIM][DIM] = {	m11, m12, m13, m14,
								m21, m22, m23, m24,
								m31, m32, m33, m34,
								m41, m42, m43, m44 };

	Set(&m[0][0]);
}

void Matrix4x4::Set(const float* m)
{
	std::memcpy(matrix, m, DIM * DIM * sizeof(float));
}

void Matrix4x4::Set(size_t row, size_t col, float val)
{
	if(!IsRowColCorrect(row, col))
		throw std::invalid_argument("Matrix4x4::Set: row or col is out of range");

	matrix[row][col] = val;
}

float Matrix4x4::Get(size_t row, size_t col) const
{
	if(!IsRowColCorrect(row, col))
		throw std::invalid_argument("Matrix4x4::Get: row or col is out of range");

	return matrix[row][col];
}

void Matrix4x4::GetRowMajor(float* m) const
{
	//std::memcpy(m, matrix, DIM * DIM * sizeof(float));

	float res[DIM*DIM];

	for(size_t row = 0; row < DIM; row++)
	{
		for(size_t col = 0; col < DIM; col++)
		{
			res[row * DIM + col] = Get(row, col);
		}
	}

	std::memcpy(m, res, DIM*DIM*sizeof(float));
}

void Matrix4x4::GetColumnMajor(float* m) const
{
	float res[DIM*DIM];

	for(size_t col = 0; col < DIM; col++)
	{
		for(size_t row = 0; row < DIM; row++)
		{
			res[col * DIM + row] = Get(row, col);
		}
	}

	std::memcpy(m, res, DIM*DIM*sizeof(float));
}

bool Matrix4x4::IsRowColCorrect(size_t row, size_t col) const
{
	return row >= 0 && row < DIM && col >= 0 && col < DIM;
}
