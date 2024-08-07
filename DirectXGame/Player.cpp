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

	if (onGround_) {
		if (velocity_.y > 0.0f ||info.LandingFlag == false ) {
			onGround_ = false;
		}

		if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {
			Vector3 acceleration = {};

			if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
				if (velocity_.x < 0.0f) {
					velocity_.x *= (1.0f - kAcceleraion);
				}
				acceleration.x += kAcceleraion;

				if (lrDirection_ != LRDirection::kRight) {
					lrDirection_ = LRDirection::kRight;
					turnFirstRotationY_ = static_cast<float>(lrDirection_);
					turnTimer_ = kTimeTurn;
				}
			} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
				acceleration.x -= kAcceleraion;

				if (lrDirection_ != LRDirection::kLeft) {
					lrDirection_ = LRDirection::kLeft;
					turnFirstRotationY_ = static_cast<float>(lrDirection_);
					turnTimer_ = kTimeTurn;
				}
			}

			if (velocity_.x < 0.0f) {
				velocity_.x *= (1.0f - kAcceleraion);
			}
			if (turnTimer_ > 0.0f) {
				turnTimer_ = turnTimer_ - 1.0f / 60.0f;

				float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
				float destionRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];
				worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destionRotationY, 1 - (turnTimer_ / kTimeTurn));
			}

			velocity_.x += acceleration.x;

			velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
		} else {
			velocity_.x *= (1.0f - kAcceleraion);
		}
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}
	}
	else {
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
	}

	if (velocity_.y < 0) {
		if (worldTransform_.translation_.y <= 2.0f) {
			info.LandingFlag = true;
		}

		if (info.LandingFlag) {
			IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPoition
			({
				worldTransform_.translation_.x, 
				worldTransform_.translation_.y + info.moveMent.y,
				worldTransform_.translation_.z
			});
			Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
			float playerBottom = (worldTransform_.translation_.y - kHeigth / 2);
			float fallDistance = rect.top - playerBottom;
			info.moveMent.y = std::min(0.0f, fallDistance);
			velocity_.x *= (1.0f - kAcceleraion);
			velocity_.y = 0.0f;
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

	CollisionMapInfo collisionMapInfo;

	collisionMapInfo.moveMent = velocity_;

	MapCollisionDetection(collisionMapInfo);


	Move(collisionMapInfo);
	SwitchingState(collisionMapInfo);
	worldTransform_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void Player::CollisonMapTop(CollisionMapInfo& info) {
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	if (info.moveMent.y <= 0) {
		return;
	}

	MapChipType mapChpiType;
	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock)
	{
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x, worldTransform_.translation_.y + info.moveMent.y, worldTransform_.translation_.z});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.y = std::max(0.0f, rect.bottom - (worldTransform_.translation_.y + kHeigth / 2));
		info.CeilingCollisionFlag = true;
	}
}

void Player::CollisonMapBottom(CollisionMapInfo& info) 
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	if (info.moveMent.y >= 0) {
		return;
	}

	MapChipType mapChpiType;
	bool hit = false;

	
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) 
	{
		hit = true;
	}

	if (!hit)
	{
		onGround_ = false;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPoition
			({
				worldTransform_.translation_.x, 
				worldTransform_.translation_.y + info.moveMent.y,
				worldTransform_.translation_.z
			});
		
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float playerBottom = (worldTransform_.translation_.y - kHeigth / 2);
		float fallDistance = rect.top - playerBottom;
		info.moveMent.y = std::min(0.0f, fallDistance);
		info.LandingFlag = true;
	}

	

}

void Player::CollisonMaplight(CollisionMapInfo& info)
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	if (info.moveMent.x <= 0) {
		return;
	}

	MapChipType mapChpiType;
	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kRightBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x + info.moveMent.x, worldTransform_.translation_.y, worldTransform_.translation_.z});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.x = std::max(0.0f, rect.left - (worldTransform_.translation_.x + kWidth / 2));
		info.WallContactFlag = true;
	}
	
}

void Player::CollisonMapLeft(CollisionMapInfo& info) 
{
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.moveMent, static_cast<Corner>(i));
	}

	if (info.moveMent.x >= 0) {
		return;
	}

	MapChipType mapChpiType;
	bool hit = false;

	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftTop]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}
	indexSet = mapChipField_->GetMapChipIndexSetByPoition(positionsNew[kLeftBottom]);
	mapChpiType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChpiType == MapChipType::kBlock) {
		hit = true;
	}

	if (hit) {
		indexSet = mapChipField_->GetMapChipIndexSetByPoition({worldTransform_.translation_.x - info.moveMent.x, worldTransform_.translation_.y, worldTransform_.translation_.z});
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.moveMent.x = std::min(0.0f, rect.right - (worldTransform_.translation_.x - kWidth / 2));
		info.WallContactFlag = true;
	}

	
}

void Player::Move(const CollisionMapInfo& info) 
{
	worldTransform_.translation_ += info.moveMent;
}

void Player::attachedCeiling(const CollisionMapInfo& info)
{
	if (info.CeilingCollisionFlag)
	{
		velocity_.y = 0;
	}
}

void Player::attachedWallCeiling(const CollisionMapInfo& info)
{
	if (info.WallContactFlag == true) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}
Vector3 Player::GetWorldPosition() { 
	Vector3 worldPos;
	
	
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;
	return worldPos;
}


void Player::OnCollision(const Enemy* enemy) {
	(void)enemy;
	velocity_ += Vector3(0, 1.0f, 0);
	isDeed_ = true;
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
