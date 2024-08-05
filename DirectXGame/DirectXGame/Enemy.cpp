#include "Enemy.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>
#include "Player.h"


Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::OnCollision(const Player* player) 
{
	(void)player; 
}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	//worldTransform_.translation_.x = 20.0f;
	worldTransform_.translation_ = position;
	//worldTransform_.translation_.y = 20.0f;

	// 速度を設定する
	velocity_ = 
	{
	    -kWalkSpeed,
	    0,
	    0,
	};

	walkTimer_ = 0;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Enemy::Update() {
	// 敵の移動処理
	worldTransform_.translation_ += velocity_;
	// タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	// 回転アニメーション
	float param = std::sin(2 * velocity_.x / velocity_.x);
	float radian = 
		kWalkMotionAngleStart + kWalkMotionAngleEnd
		* (param + 1.0f) / 2.0f;

	// 度をラジアンに変換
	param = std::lerp(1.0f,1.0f,radian);

	worldTransform_.UpdateMatrix();

}


void Enemy::Draw() 
{
	model_->Draw(worldTransform_, *viewProjection_); 
}

AABB Enemy::GetAABB() {
	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeigth / 2.0f, worldPos.z - kWidth / 2.0f};

	aabb.max = {
	    worldPos.x + kWidth / 2.0f,
	    worldPos.y + kHeigth / 2.0f,
	    worldPos.z + kWidth / 2.0f,
	};

	return aabb;
}

Vector3 Enemy::GetWorldPosition() 
{ // ワールド座標を入れる変数
	Vector3 worldPos;

	// ワールド行列の平行移動分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}