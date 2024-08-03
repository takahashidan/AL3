#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "AABB.h"
//#include <GetWorldPosition.h>

class MapChipField;
class Enemy;
class GetWorldPosition;

enum class LRDirection
{
	kRight,
	kLeft,
};

enum Corner {
	kRightBottom, 
	kLeftBottom,  
	kRightTop,    
	kLeftTop,   

	kNumCorner 
};

struct CollisionMapInfo {
	bool CeilingCollisionFlag = false; 
	bool LandingFlag = false;          
	bool WallContactFlag = false;      
	Vector3 moveMent;                  
};


class Player 
{

public:
	Player();
	~Player();

	WorldTransform& GetWorldTrnsform();

	void SetMapChipField(MapChipField* mapChipField) 
	{ 
		mapChipField_ = mapChipField; 
	}

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	// マップ衝突判定
	void MapCollisionDetection(CollisionMapInfo& info);

	void CollisonMapTop(CollisionMapInfo& info);
	void CollisonMapBottom(CollisionMapInfo& info); 
	void CollisonMaplight(CollisionMapInfo& info);
	void CollisonMapLeft(CollisionMapInfo& info);

	void Move(const CollisionMapInfo& info);
	void attachedCeiling(const CollisionMapInfo& info);
	void SwitchingState(CollisionMapInfo& info);
	void attachedWallCeiling(const CollisionMapInfo& info);

	

	AABB GetAABB();
	
	Vector3 GetWorldPosition();

	void OnCollision(const Enemy* enemy);

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	MapChipField* mapChipField_ = nullptr;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	LRDirection lrDirection_ = LRDirection::kLeft;

	static inline const float kAcceleraion = 0.3f;
	static inline const float kLimitRunSpeed = 0.3f;
	Vector3 velocity_ = {};

	float turnFirstRotationY_ = 0.0f;
	float turnTimer_ = 0.0f;
	static inline const float kTimeTurn = 0.3f;

	bool onGround_ = true;

	static inline const float kGravityAcceleration = 0.2f;
	static inline const float kLimitFallSpeed = 1.0f;
	static inline const float kJumpAcceleration = 1.0f;

	const Vector3& GetVelocity() const { return velocity_; }

	static inline const float kWidth = 1.6f;
	static inline const float kHeigth = 1.6f;
	
	static inline const float kBlank = 0.1f;

	static inline const float kAttenuationLanding = 1.0f;
	static inline const float kAttenuationWall = 1.0f;


	
};
