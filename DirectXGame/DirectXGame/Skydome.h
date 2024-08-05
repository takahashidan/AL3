#pragma once
#include "Model.h"
#include "WorldTransform.h"


class Skydome 
{
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, ViewProjection* viewProjection);
	
	Skydome();

	~Skydome();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	private:
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	

	Model* model_ = nullptr;
	
};
