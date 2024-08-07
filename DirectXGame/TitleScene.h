#pragma once
#include "Input.h"
#include "Player.h"
#include "Model.h"
#include "MapChipField.h"
#include "DirectXCommon.h"
#include "ViewProjection.h"

class TitleScene 
{
public:
	TitleScene();
	~TitleScene();
	void Initialize();

	void Update();

	void Draw();

	bool finished_ = false;
	bool IsFinished() const { return isFinished_; }

	private:

	bool isFinished_;

private:

	Input* input_ = nullptr; 
	Player* player_ = nullptr;
	DirectXCommon* dxCommon_ = nullptr;
	MapChipField* mapChipField_;

	WorldTransform worldTransform_;

	Model* model_ = nullptr;

	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = {};
};
