#include "DeathParticles.h"
#include <cassert>
#include <numbers>
#include <Matrix4x4.h>
#include <algorithm>




void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	//ワールド変換の初期化
	for (auto& worldTransform : worldTransforms_)
	{
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;

	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};

}

void DeathParticles::Update() 
{
	// ワールド変換の更新
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

	 //カウンターを1フレーム分の秒数進める
	counter_ += 1.0f / 60.0f;

	// 存続時間上限に達したら
	if (counter_ >= kDuration)
	{
		counter_ = kDuration;
		// 終了扱いにする
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
	// モデルの描画
	for (auto& worldTransform : worldTransforms_) 
	{
		//model_->Draw(worldTransform, *viewProjection_, textureHandle_);
	
		model_->Draw(worldTransform, *viewProjection_, &objectColor_);
	}

	//if (isFinished_) 
	//{
	//	return;
	//}
}
