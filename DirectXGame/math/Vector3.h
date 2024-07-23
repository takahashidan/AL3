#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final 
{
	float x;
	float y;
	float z;

	// +=演算子のオーバーロード
	Vector3& operator+=(const Vector3& a);
	Vector3& operator-=(const Vector3& i);
};
