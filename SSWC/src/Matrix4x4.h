#pragma once

#include <cstring>
#include <stdexcept>

#define DIM 4

class Matrix4x4
{
public:
	Matrix4x4();
	Matrix4x4(const float* m);
	Matrix4x4(const float m[DIM][DIM]);
	Matrix4x4(float m11, float m12, float m13, float m14,
			  float m21, float m22, float m23, float m24,
			  float m31, float m32, float m33, float m34,
			  float m41, float m42, float m43, float m44);

	void Set(const float* m);
	void Set(size_t row, size_t col, float val);
	float Get(size_t row, size_t col) const;

	void GetRowMajor(float* m) const;
	void GetColumnMajor(float* m) const;

	bool IsRowColCorrect(size_t row, size_t col) const;

private:
	float matrix[DIM][DIM];

public:
	static const Matrix4x4 Identity;
	static const Matrix4x4 Zero;
};