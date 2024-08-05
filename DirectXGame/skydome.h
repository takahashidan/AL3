#pragma once
#include "Model.h"
#include "WorldTransform.h"


class Skydome 
{
public:

	void Initialize(Model* model, ViewProjection* viewProjection);
	
	Skydome();

	~Skydome();
	void Update();

	void Draw();

	private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	

	Model* model_ = nullptr;
	
};
