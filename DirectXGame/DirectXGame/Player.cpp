#define NOMINMAX
#include "Player.h"
#include "MapChipField.h"
#include "Player.h"
#include "Rect.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <Input.h>
#include <algorithm>
#include <cassert>
#include <numbers>



Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	assert(model);
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	model_ = model;

	// 引数の内容をメンバ変数に記録
	viewProjection_ = viewProjection;
}

WorldTransform& Player::GetWorldTrnsform() { return worldTransform_; }

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeigth / 2.0f, 0}, //  kRightBottom
	    {-kWidth / 2.0f, -kHeigth / 2.0f, 0}, //  kLeftBottom
	    {+kWidth / 2.0f, +kHeigth / 2.0f, 0}, //  kRightTop
	    {-kWidth / 2.0f, +kHeigth / 2.0f, 0}, //  kLeftTop
	};
	return center + offsetTable[static_cast<uint32_t>(corner)];
}

void Player::SwitchingState(CollisionMapInfo& info) {

	// ①移動入力
	// 接地状態
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f ||info.LandingFlag == false ) {
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

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					// 旋回開始の角度を記録する
					turnFirstRotationY_ = static_cast<float>(lrDirection_);
					// 旋回タイマーに時間を設定する
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleraion;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
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
				worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destionRotationY, 1 - (turnTimer_ / kTimeTurn));
			}

			// 加速/減速
			velocity_.x += acceleration.x;

			// 最大速度制限
			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			// 非入力時は移動減衰をかける
			velocity_.x *= (1.0f - kAcceleraion);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	}
	// 空中
	else {
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		// velocity_.y = std::max(velocity_.y, -kLimitFallSpeed);
	}

	// 地面との当たり判定
	// 降下中?
	if (velocity_.y < 0) {
		// Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			info.LandingFlag = true;
		}

		if (info.LandingFlag) {
			IndexSet indexSet;
			//プレイヤーの着地処理
			indexSet = mapChipField_->GetMapChipIndexSetByPoition
			({
				worldTransform_.translation_.x, 
				worldTransform_.translation_.y + info.moveMent.y,
				worldTransform_.translation_.z
			});
			//  めり込み先ブロックの範囲短形
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float playerBottom = (worldTransform_.translation_.y - kHeigth / 2);
			float fallDistance = rect.top - playerBottom;
			info.moveMent.y = std::min(0.0f, fallDistance);
			// 摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAcceleraion);
			
			// 下方向速度をリセット
			velocity_.y = 0.0f;
			
			// 接地状態に移行
			onGround_ = true;
		}
	}

	
}



void Player::MapCollisionDetection(CollisionMapInfo& info) 
{ 
	CollisonMapTop(info); 
	CollisonMapBottom(info);
	CollisonMaplight(info);
	CollisonMapLeft(info);
}

void Player::Update() {

	// ②.1移動情報初期化
	CollisionMapInfo collisionMapInfo;

	// 移動量に速度の値をコピー
	collisionMapInfo.moveMent = velocity_;

	// マップ衝突チェック
	MapCollisionDetection(collisionMapInfo);


	Move(collisionMapInfo);
	SwitchingState(collisionMapInfo);
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();
	// 行列計算
	worldTransform_.UpdateMatrix();
}

// ②.上方向衝突判定
void Player::CollisonMapTop(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	// 上昇あり?
	if (info.moveMent.y <= 0) {
		return;
	}

	MapChipType mapChpiType;
	// 真上の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}
	// 右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock)
	{
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x, worldTransform_.translation_.y + info.moveMent.y, worldTransform_.translation_.z});
		// めり込み先ブロックの範囲短形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.y = std::max(0.0f, rect.bottom - (worldTransform_.translation_.y + kHeigth / 2));
		// 天井に当たったことを記録する.
		info.CeilingCollisionFlag = true;
	}
}

// ②.下方向衝突判定
void Player::CollisonMapBottom(CollisionMapInfo& info) 
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	// 降下あり?
	if (info.moveMent.y >= 0) {
		return;
	}

	MapChipType mapChpiType;
	// 真下の当たり判定を行う
	bool hit = false;

	
	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}

	// 落下開始
	if (!hit)
	{
		//空中状態に切り替える
		onGround_ = false;
	}

	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPoition
			({
				worldTransform_.translation_.x, 
				worldTransform_.translation_.y + info.moveMent.y,
				worldTransform_.translation_.z
			});
		
		// めり込み先ブロックの範囲短形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float playerBottom = (worldTransform_.translation_.y - kHeigth / 2);
		float fallDistance = rect.top - playerBottom;
		info.moveMent.y = std::min(0.0f, fallDistance);
		// 天井に当たったことを記録する.
		info.LandingFlag = true;
	}

	

}

// ②.右方向衝突判定 
void Player::CollisonMaplight(CollisionMapInfo& info)
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	// 右移動あり?
	if (info.moveMent.x <= 0) {
		return;
	}

	MapChipType mapChpiType;
	// 右の当たり判定を行う
	bool hit = false;

	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x + info.moveMent.x, worldTransform_.translation_.y, worldTransform_.translation_.z});
		// めり込み先ブロックの範囲短形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.x = std::max(0.0f, rect.left - (worldTransform_.translation_.x + kWidth / 2));
		// 壁に当たったことを記録する.
		info.WallContactFlag = true;
	}
	
}

// ②.左方向衝突判定
void Player::CollisonMapLeft(CollisionMapInfo& info) 
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	// 左移動あり?
	if (info.moveMent.x >= 0) {
		return;
	}

	MapChipType mapChpiType;
	// 左の当たり判定を行う
	bool hit = false;

	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット?
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x - info.moveMent.x, worldTransform_.translation_.y, worldTransform_.translation_.z});
		// めり込み先ブロックの範囲短形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.x = std::min(0.0f, rect.right - (worldTransform_.translation_.x - kWidth / 2));
		// 壁に当たったことを記録する.
		info.WallContactFlag = true;
	}

	
}

//③判定結果を反映して移動させる
void Player::Move(const CollisionMapInfo& info) 
{
	//移動
	worldTransform_.translation_ += info.moveMent;
}

//④天井に接してる場合の処理
void Player::attachedCeiling(const CollisionMapInfo& info)
{
	//天井にあたった?
	if (info.CeilingCollisionFlag)
	{
		velocity_.y = 0;
	}
}

// ④壁に接してる場合の処理
void Player::attachedWallCeiling(const CollisionMapInfo& info)
{
	if (info.WallContactFlag == true) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}
Vector3 Player::GetWorldPosition() { // ワールド座標を入れる変数
	Vector3 worldPos;
	
	
	// ワールド行列の平行移動分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}


void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	// ジャンプ開始(仮)
	velocity_ += Vector3(0, 1.0f, 0);
	
}



void Player::Draw() { model_->Draw(worldTransform_, *viewProjection_, textureHandle_); }



AABB Player::GetAABB() {

	Vector3 worldPos = GetWorldPosition();
	AABB aabb;

	aabb.min =
	{
		worldPos.x - kWidth / 2.0f,
		worldPos.y - kHeigth / 2.0f,
		worldPos.z - kWidth / 2.0f
	};
	
	aabb.max =
	{ worldPos.x + kWidth / 2.0f,
	  worldPos.y + kHeigth / 2.0f,
	  worldPos.z + kWidth / 2.0f,
	};

	return aabb;
}
