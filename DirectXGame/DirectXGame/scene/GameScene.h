#pragma once
#include "MapChipField.h"
#include "Audio.h"
#include "DirectXCommon.h"
#include "Skydome.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include "Player.h"
#include "Enemy.h"
#include "CameraController.h"
#include <cassert>
#include "DeathParticles.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数

	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	void ChecAllCollisiions();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	void GenerateBlocks();
	bool IsCollision(AABB aabb1, AABB aabb2);

private: // メンバ変数

	WorldTransform worldTransform_;
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr; 
	Audio* audio_ = nullptr;
	Model* modelBlock_ = nullptr;
	Model* modelSkydome_ = nullptr;
	MapChipField* mapChipField_;
	Skydome* skydome_ = nullptr;
	Player* player_ = nullptr;
	Enemy* enemy_ = nullptr;
	Model* model_ = nullptr;
	Model* enmeyModel_ = nullptr;
	CameraController* CameraController_ = nullptr;

	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	WorldTransform wolrldTransform_ = {};
	ViewProjection viewProjection_ = {}; 
	MapChipData mapChipData_ = {};
	uint32_t numBlockVirtical_ = 0;
	uint32_t numBlockHorizontal_ = 0;
	uint32_t tetureHandle_ = 0;
	
	std::list<Enemy*> enemies_;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	// デバックカメラ有効
	bool isDebugCameraActiive_ = false;

	//デバックカメラ
	DebugCamera* debugCamera_ = nullptr;

	//デバックカメラのビュープロジェクション
	ViewProjection* debugViewProjection_; 

	DeathParticles* deathParticles_ ;
	//デスパーティクルが存在するか
	bool deathParticlesFlag = false;

};
