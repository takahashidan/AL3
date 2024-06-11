#include "Player.h"
#include <cassert>
#include<numbers>

void Player::Initialize(Model*model,ViewProjection*viewProjection,const Vector3& position) { 
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() {
	worldTransform_.TransferMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}
