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

	// +演算子のオーバーロード
	Vector3 operator+(const Vector3& other) const {
		Vector3 result = *this; // 現在のオブジェクトのコピーを作成
		result += other;        // +=演算子を使用してotherを追加
		return result;          // 結果を返す
	}


};
