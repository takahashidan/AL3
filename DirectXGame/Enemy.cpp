#include "Enemy.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = 
	{
	    -kWalkSpeed,
	    0,
	    0,
	};

	walkTimer_ = 0;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	viewProjection_ = viewProjection;
}

void Enemy::Update() {
	worldTransform_.translation_ += velocity_;
	walkTimer_ += 1.0f / 60.0f;

	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	// 回転アニメーション
	float param = std::sin(2 * velocity_.x / velocity_.x);
	float radian = 
		kWalkMotionAngleStart + kWalkMotionAngleEnd
		* (param + 1.0f) / 2.0f;

	param = std::lerp(1.0f,1.0f,radian);

	worldTransform_.UpdateMatrix();

}


void Enemy::Draw() 
{
	model_->Draw(worldTransform_, *viewProjection_); 
}

