#define NOMINMAX
#include "Player.h"
#include "WorldTransform.h"
#include "ViewProjection.h" 
#include <cassert>
#include <numbers>
#include <Input.h>
#include <algorithm>


Player::Player() {}

Player::~Player() {}

WorldTransform& Player::GetWorldTrnsform() {
	// TODO: return ステートメントをここに挿入します
	//Vector3 worldPosition = {};
	//worldPosition.x = worldTransfrom_.translation_.x;
	//worldPosition.y = worldTransfrom_.translation_.y;
	//worldPosition.z = worldTransfrom_.translation_.z;
	//
	//return worldPosition;
	return worldTransfrom_;
}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransfrom_.Initialize();
	worldTransfrom_.translation_ = position;
	worldTransfrom_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

void Player::Update() 
{
	// 行列を定数バッファに転送
	worldTransfrom_.TransferMatrix();
	bool landing = false;

	// 移動入力
	// //接地状態
	if (onGround_) {
		//ジャンプ開始
		if (velocity_.y > 0.0f)
		{
			// 空中状態に移行
			onGround_ = false;
		} 
		


		// 左右移動操作
		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			// 左右加速
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				// 左移動中の右入力
				if (velocity_.x < 0.0f) {
					// 速度と逆方向に入力中は急ブレーキ
					velocity_.x *= (1.0f - kAcceleraion);
				}
				acceleration.x += kAcceleraion;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					// 旋回開始の角度を記録する
					turnFirstRotationY_ = static_cast<float>(lrDirection_);
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleraion;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始の角度を記録する
					turnFirstRotationY_ = static_cast<float>(lrDirection_);
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			}

			// 右移動中の左入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAcceleraion);
			}

			// 旋回制御
			if (turnTimer_ > 0.0f) {
				turnTimer_ = turnTimer_ - 1.0f / 60.0f;

				// 左右の自キャラ角度テーブル
				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
				// 状態に応じえた角度を取得する
				float destionRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				// 自キャラの角度を設定する
				worldTransfrom_.rotation_.y = std::lerp(turnFirstRotationY_, destionRotationY, 1 - (turnTimer_ / kTimeTurn));
			}

			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAcceleraion);
		}
		if (Input::GetInstance()->PushKey(DIK_UP))
		{
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	}
	// 空中
	else 
	{
		// 落下速度
		velocity_ = Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 地面との当たり判定
	// 降下中?
	if (velocity_.y < 0)
	{
		// Y座標が地面いかになったら着地
		if (worldTransfrom_.translation_.y <= 1.0f)
		{
			landing = true;
		}
		if (landing) {
			// めり込み排斥
			worldTransfrom_.translation_.y = 1.0f;
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAcceleraion);
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			// 接地状態に移行
			onGround_ = true;
		}
	}

	//移動
	worldTransfrom_.translation_ += velocity_;
	//行列計算
	worldTransfrom_.UpdateMatrix();

	

}

void Player::Draw()
{ 
	model_->Draw(worldTransfrom_, *viewProjection_, textureHandle_); }
//WorldTransform Player::GetWorldTrnsform() 
//{
//	Vector3 worldPosition;
//	worldPosition.x = worldTransfrom_.translation_.x;
//	worldPosition.y = worldTransfrom_.translation_.y;
//	worldPosition.z = worldTransfrom_.translation_.z;
//
//	return worldPosition; 
//}

