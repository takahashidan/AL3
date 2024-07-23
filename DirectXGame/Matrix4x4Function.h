#pragma once

#include <cmath>
#include "Vector3.h"

class Matrix4x4Function {
public:
	float m[4][4];
;
	static Matrix4x4Function MakeScaleMatrix(const Vector3& scale);
	static Matrix4x4Function MakeRotateXMatrix(float radian);
	static Matrix4x4Function MakeRotateYMatrix(float radian);
	static Matrix4x4Function MakeRotateZMatrix(float radian);
	static Matrix4x4Function Multiply(Matrix4x4Function m1, Matrix4x4Function m2);
	static Matrix4x4Function MakeRotationMatrix(const Vector3& rotate);
	static Matrix4x4Function MakeTranslationMatrix(const Vector3& translate);
	static Matrix4x4Function MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	Matrix4x4Function operator*(const Matrix4x4Function& other) const;
};

