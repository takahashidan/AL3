#include "Matrix4x4Function.h"

Matrix4x4Function Matrix4x4Function::MakeScaleMatrix(const Vector3& scale) {
	Matrix4x4Function scaleMatrix{};

	scaleMatrix.m[0][0] = scale.x;
	scaleMatrix.m[1][1] = scale.y;
	scaleMatrix.m[2][2] = scale.z;
	scaleMatrix.m[3][3] = 1.0f;
	return scaleMatrix;
}

Matrix4x4Function Matrix4x4Function::MakeRotateXMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4Function rotateXMatrix{};
	rotateXMatrix.m[0][0] = 1.0f;
	rotateXMatrix.m[1][1] = cosTheta;
	rotateXMatrix.m[1][2] = sinTheta;
	rotateXMatrix.m[2][1] = -sinTheta;
	rotateXMatrix.m[2][2] = cosTheta;
	rotateXMatrix.m[3][3] = 1.0f;
	return rotateXMatrix;
}

Matrix4x4Function Matrix4x4Function::MakeRotateYMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4Function rotateYMatrix{};
	rotateYMatrix.m[0][0] = cosTheta;
	rotateYMatrix.m[0][2] = -sinTheta;
	rotateYMatrix.m[1][1] = 1.0f;
	rotateYMatrix.m[2][0] = sinTheta;
	rotateYMatrix.m[2][2] = cosTheta;
	rotateYMatrix.m[3][3] = 1.0f;
	return rotateYMatrix;
}

Matrix4x4Function Matrix4x4Function::MakeRotateZMatrix(float radian) {
	float cosTheta = std::cos(radian);
	float sinTheta = std::sin(radian);
	Matrix4x4Function rotateZMatrix{};
	rotateZMatrix.m[0][0] = cosTheta;
	rotateZMatrix.m[0][1] = sinTheta;
	rotateZMatrix.m[1][0] = -sinTheta;
	rotateZMatrix.m[1][1] = cosTheta;
	rotateZMatrix.m[2][2] = 1.0f;
	rotateZMatrix.m[3][3] = 1.0f;
	return rotateZMatrix;
}

Matrix4x4Function Matrix4x4Function::Multiply(Matrix4x4Function m1, Matrix4x4Function m2) {
	Matrix4x4Function result{};
	for (int i = 0; i < 4; ++i) 
	{
		for (int j = 0; j < 4; ++j)
		{
			result.m[i][j] = m1.m[i][0] * m2.m[0][j] + m1.m[i][1] * m2.m[1][j] + m1.m[i][2] * m2.m[2][j] + m1.m[i][3] * m2.m[3][j];
		}
	}
	return result;
}

Matrix4x4Function Matrix4x4Function::MakeRotationMatrix(const Vector3& rotate) {
	Matrix4x4Function rotateXMatrix = MakeRotateXMatrix(rotate.x);
	Matrix4x4Function rotateYMatrix = MakeRotateYMatrix(rotate.y);
	Matrix4x4Function rotateZMatrix = MakeRotateZMatrix(rotate.z);
	Matrix4x4Function a = rotateXMatrix * rotateYMatrix * rotateZMatrix;
	return a;
}

Matrix4x4Function Matrix4x4Function::MakeTranslationMatrix(const Vector3& translate) {
	Matrix4x4Function translationMatrix{};
	translationMatrix.m[0][0] = 1.0f;
	translationMatrix.m[1][1] = 1.0f;
	translationMatrix.m[2][2] = 1.0f;
	translationMatrix.m[3][3] = 1.0f;
	translationMatrix.m[3][0] = translate.x;
	translationMatrix.m[3][1] = translate.y;
	translationMatrix.m[3][2] = translate.z;
	return translationMatrix;
}

Matrix4x4Function Matrix4x4Function::MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
	Matrix4x4Function scaleMatrix = MakeScaleMatrix(scale);
	Matrix4x4Function rotationMatrix = MakeRotationMatrix(rotate);
	Matrix4x4Function translationMatrix = MakeTranslationMatrix(translate);
	return (scaleMatrix * rotationMatrix * translationMatrix);
}

Matrix4x4Function Matrix4x4Function::operator*(const Matrix4x4Function& other) const { 
	return Multiply(*this, other); 
}

