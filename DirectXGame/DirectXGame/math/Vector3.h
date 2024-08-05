#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	Vector3& operator+=(const Vector3& a);

	Vector3& operator-=(const Vector3& i);

};
	Vector3 operator+(const Vector3& a, const Vector3& b);