#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"

//前方宣言
class Player;

// 短形
struct Rect {
	float left = 0.0f;   //左端
	float rigth = 1.0f;  //右端
	float bottom = 0.0f; //下端
	float top = 1.0f;    //上端
};

class CameraController
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	Vector3 Lerp(const Vector3& a, const Vector3& b, float t) 
	{ 
		return {t * a.x + (1.0f - t) * b.x, t * a.y + (1.0f - t) * b.z, t * a.z + (1.0f - t) * b.z}; 
	}

	/// <summary>
	/// ビュープロジェクションを取得
	/// </summary>
	/// <returns>ビュープロジェクション</returns>
	const ViewProjection& GetViewProjection() { return viewProjection_; }


	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();
	void SetMovableArea(Rect area) { movableArea_ = area; }


	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private:

	ViewProjection viewProjection_;
	WorldTransform* worldTransform_;
	Player* target_ = nullptr;

	// 追跡対象とカメラの座標の差(オフセット)
	Vector3 targetOffset_ = {0, 0, -50.0f};

	// カメラ移動範囲
	Rect movableArea_ = 
	{
		0,    //左端
		100, //右端
		0,    //下端
		1000  //上端
	};

	// カメラの目標座標
	Vector3 cameraTargetCoordinates;
	//座上補間割合
	static inline const float kInterpolationRate = 0.8f;

};

