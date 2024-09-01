#pragma once
#include "Vector3.h"
struct Matrix4x4 final {
	float m[4][4];

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale);
	static Matrix4x4 MakeRotateXMatrix(float radian);
	static Matrix4x4 MakeRotateYMatrix(float radian);
	static Matrix4x4 MakeRotateZMatrix(float radian);
	static Matrix4x4 Multiply(Matrix4x4 m1, Matrix4x4 m2);
	static Matrix4x4 MakeRotationMatrix(const Vector3& rotate);
	static Matrix4x4 MakeTranslationMatrix(const Vector3& translate);
	static Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);
	static Vector3 Transform(const Vector3& vector, const Matrix4x4& matrix);
	Matrix4x4 operator*(const Matrix4x4& other) const;
};
