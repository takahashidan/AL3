#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <AABB.h>
#include <GetWorldPosition.h>


class MapChipField;
class Player;


/// <summary>
/// 敵
/// </summary>

class Enemy 
{
public:

	Enemy();
	~Enemy();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	// 衝突応答
	void OnCollision(const Player* player);


	AABB GetAABB();

	// ワールド座標を取得
	Vector3 GetWorldPosition();

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

	// 歩行の速さ
	static inline const float kWalkSpeed = 0.1f;

	// 速度
	Vector3 velocity_ = {};

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.0f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 90.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 3.0f;
	// 経過時間
	float walkTimer_ = 0.0f;

	// キャラキターの当たり判定サイズ
	static inline const float kWidth = 1.6f;
	static inline const float kHeigth = 1.6f;


};
