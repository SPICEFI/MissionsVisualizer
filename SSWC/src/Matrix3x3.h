#pragma once

#include <cstring>

class Matrix3x3
{
public:
	Matrix3x3();
	Matrix3x3(const float* m);
	Matrix3x3(const float m[3][3]);

	void Set(const float* m);
	void Set(size_t row, size_t col, float val);
	const float* Get() const;
	float Get(size_t row, size_t col) const;

private:
	float matrix[3][3];

public:
	static const Matrix3x3 Identity;
};