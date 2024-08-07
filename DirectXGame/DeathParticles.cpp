#include "DeathParticles.h"
#include <cassert>
#include <numbers>
#include <Matrix4x4.h>
#include <algorithm>




bool DeathParticles::IsFinished() const 
{ 
	 return isFinished_; 
}

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	for (auto& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	viewProjection_ = viewProjection;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

void DeathParticles::Update() 
{
	for (auto& worldTransform : worldTransforms_) {
		worldTransform.UpdateMatrix();
	}

	for (uint32_t i = 0; i < 8; i++)
	{
		Vector3 velocity = {kSpeed, 0, 0};
		float angle = kAngleUnit * (float)i;
		Matrix4x4 matrixRotation = Matrix4x4::MakeRotateZMatrix(angle);
		velocity = Matrix4x4::Transform(velocity, matrixRotation);
		worldTransforms_[i].translation_ += velocity;
	}

	counter_ += 1.0f / 60.0f;

	if (counter_ >= kDuration)
	{
		counter_ = kDuration;
		isFinished_ = true;
	}
	
	if (isFinished_)
	{
		return;
	}

	color_.w = std::clamp(1 - counter_, 0.0f, 1.0f);
	objectColor_.SetColor(color_);
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() 
{ 
	for (auto& worldTransform : worldTransforms_) 
	{
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}

	if (isFinished_) 
	{
		return;
	}
}
