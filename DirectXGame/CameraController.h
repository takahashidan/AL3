#pragma once
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "MapChipField.h"
#include "Rect.h"

class Player;


class CameraController
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	void SetTarget(Player* target) { target_ = target; }
	void Reset();
	Vector3 Lerp(const Vector3& a, const Vector3& b, float t) 
	{ 
		return {t * a.x + (1.0f - t) * b.x, t * a.y + (1.0f - t) * b.z, t * a.z + (1.0f - t) * b.z}; 
	}

	const ViewProjection& GetViewProjection() { return viewProjection_; }


	void Update();
	void SetMovableArea(Rect area) { movableArea_ = area; }


	void Draw();


private:

	ViewProjection viewProjection_;
	WorldTransform* worldTransform_;
	Player* target_ = nullptr;

	Vector3 targetOffset_ = {0, 0, -50.0f};

	Rect movableArea_ = 
	{
		0,    
		100, 
		0,    
		1000  
	};

	Vector3 cameraTargetCoordinates;
	static inline const float kInterpolationRate = 0.8f;

	static inline const float kVelocityBias = 1.2f;
	static inline const Rect Margin = {0, 0, 0, 0};


};

