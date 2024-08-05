#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <MapChipField.h>
#include <array>
#include <math.h>
#include <ObjectColor.h>


class DeathParticles 
{
public:

	

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
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ビュープロジェクション
	ViewProjection* viewProjection_ = nullptr;

	//パーティクルの個数
	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	// 存続時間
	static inline const float kDuration = 3.0f;
	// 移動の速さ
	static inline const float kSpeed = 0.5f;
	// 分割した1個分の角度
	static inline const float kAngleUnit = 2.0f * 3.14f / 8.0f;
	
	// 終了フラグ
	bool isFinished_ = false;
	// 経過時間カウント
	float counter_ = 0.0f;
	
	ObjectColor objectColor_;
	Vector4 color_;
};
