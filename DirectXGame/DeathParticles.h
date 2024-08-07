#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include <MapChipField.h>
#include <array>
#include <math.h>
#include <ObjectColor.h>


class DeathParticles 
{
public:

	bool IsFinished() const;

	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	void Update();

	void Draw();

private:
	MapChipField* mapChipField_ = nullptr;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;

	static inline const uint32_t kNumParticles = 8;

	std::array<WorldTransform, kNumParticles> worldTransforms_;

	static inline const float kDuration = 3.0f;
	static inline const float kSpeed = 0.5f;
	static inline const float kAngleUnit = 2.0f * 3.14f / 8.0f;
	
	bool isFinished_ = false;
	float counter_ = 0.0f;
	
	ObjectColor objectColor_;
	Vector4 color_;
};
