#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <AABB.h>
#include <GetWorldPosition.h>


class MapChipField;
class Player;



class Enemy 
{
public:

	Enemy();
	~Enemy();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }
	void OnCollision(const Player* player);


	AABB GetAABB();

	Vector3 GetWorldPosition();

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	MapChipField* mapChipField_ = nullptr;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	static inline const float kWalkSpeed = 0.1f;

	Vector3 velocity_ = {};

	static inline const float kWalkMotionAngleStart = 0.0f;
	static inline const float kWalkMotionAngleEnd = 90.0f;
	static inline const float kWalkMotionTime = 3.0f;
	float walkTimer_ = 0.0f;

	static inline const float kWidth = 1.6f;
	static inline const float kHeigth = 1.6f;


};
