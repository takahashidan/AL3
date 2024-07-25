#pragma once
#include "Model.h"
#include "WorldTransform.h"

// 左右
enum class LRDirection
{
	kRight,
	kLeft,
};

class Player 
{

public:
	Player();
	~Player();

	WorldTransform& GetWorldTransform();

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="model">モデル</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransfrom_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	LRDirection lrDirection_ = LRDirection::kLeft;

	static inline const float kAcceleraion = 0.3f;
	static inline const float kLimitRunSpeed = 0.8f;
	Vector3 velocity_ = {};

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	//重力加速度(下方向)
	static inline const float kGravityAcceleration = 0.2f;
	// 最大落下速度(下方向)
	static inline const float kLimitFallSpeed = 2.0f;
	// ジャンプ速度(上方向)
	static inline const float kJumpAcceleration = 2.0f;


};
