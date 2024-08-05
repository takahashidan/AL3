#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"
#include "Skydome.h"
#include "TextureManager.h"
#include "CameraController.h"
#include <cassert>
#include <AABB.h>

GameScene::GameScene() {}

GameScene::~GameScene()
{
	for (std::vector<WorldTransform*>& worldTansformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTansformBlock : worldTansformBlockLine) {
			delete worldTansformBlock;
		}
	}

	for (Enemy* kenemise_ : enemies_) 
	{
		delete kenemise_;
	}

	delete deathParticles_;


	worldTransformBlocks_.clear();
	delete modelBlock_;
	delete modelSkydome_;
	delete mapChipField_;
	delete skydome_;
	delete debugCamera_;
	delete player_;
	delete enemy_;
	
}

void GameScene::ChecAllCollisiions()
{
	AABB aabb1, aabb2;

	aabb1 = player_->GetAABB();

	for (Enemy* enemy : enemies_)
	{
		aabb2 = enemy->GetAABB();
		
		if (IsCollision(aabb1, aabb2))
		{

			player_->OnCollision(enemy);
			enemy_->OnCollision(player_);
		}
	}

}

void GameScene::Initialize() {


	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	tetureHandle_ = TextureManager::Load("sample.png");


	model_ = Model::CreateFromOBJ("player", true);
	enmeyModel_ = Model::CreateFromOBJ("enemy", true);
	player_ = new Player();
	enemy_  = new Enemy();
	modelBlock_ = Model::Create();
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1,18);
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(10, 18);
	
	player_->Initialize(model_, &viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	enemy_->Initialize(enmeyModel_, &viewProjection_, enemyPosition);

	enemy_->SetMapChipField(mapChipField_);

	for (int32_t i = 1; i < 4; ++i)
	{
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition_ = 
		{ 
			enemyPosition.x * i, enemyPosition.y * i, enemyPosition.z 
		};
		newEnemy->Initialize(enmeyModel_, &viewProjection_, enemyPosition_);

		enemies_.push_back(newEnemy);
	}



	worldTransform_.Initialize();

	const uint32_t kNumBlockVirtical = 20;
	const uint32_t kNumBlockHorizontal = 100;

	numBlockVirtical_ = 20;
	numBlockHorizontal_ = 100;

	worldTransformBlocks_.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	}

	skydome_ = new Skydome();
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_->Initialize(modelSkydome_, &viewProjection_);

	wolrldTransform_.Initialize();
	viewProjection_.Initialize();
	viewProjection_.translation_.y = 10;
	viewProjection_.translation_.x = 20;

	debugCamera_ = new DebugCamera(1280, 720);

	 GenerateBlocks();


	CameraController_ = new CameraController; // 生成
	CameraController_->Initialize();          // 初期化
	CameraController_->SetTarget(player_);    // 追跡対象をリセット
	CameraController_->Reset();               // リセット(瞬間合わせ)

	Rect setter = 
	{
		35.5,    //左端
		160.5,   //右端
		19.5, 	 //下端
		19.0	 //上端
	}; 

	CameraController_->SetMovableArea(setter);

	deathParticles_ = new DeathParticles;
	
	deathParticles_->Initialize(model_, &viewProjection_, playerPosition);
	
}

void GameScene::Update() {
	skydome_->Update();
	// 自キャラの更新
	player_->Update();
	// 敵キャラの更新
	enemy_->Update();
	for (Enemy* kenemise_ : enemies_) {
		kenemise_->Update();
	}


	deathParticles_->Update();

	// 全ての当たり判定を行う
	ChecAllCollisiions();

	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock) {
				continue;
			} else {
				worldTransformBlock->UpdateMatrix();
			}
		}
	}

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActiive_ = true;
	}
#endif

	debugCamera_->Update();

	if (isDebugCameraActiive_) {
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	} else {

		CameraController_->Update();
		viewProjection_.matView = CameraController_->GetViewProjection().matView;
		viewProjection_.matProjection = CameraController_->GetViewProjection().matProjection;
		viewProjection_.TransferMatrix();
	}

	


}

void GameScene::Draw()
{

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw();
	// 敵キャラの描画
	enemy_->Draw();
	for (Enemy* kenemise_ : enemies_) {
		kenemise_->Draw();
	}
	// 天球の描画
	skydome_->Draw();

	// マップチップの描画
	for (std::vector<WorldTransform*> worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	//for (DeathParticles* deathParticlesHati : deathParticles_) 
	//{
	//	if (deathParticlesFlag) 
	//	{
	//		deathParticlesHati->Draw();
	//	}
	//}

	deathParticles_->Draw();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();

#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GenerateBlocks() {
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	worldTransformBlocks_.resize(numBlockVirtical);

	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}

	 for (uint32_t x = 0; numBlockVirtical > x;x++)
	{
		for (uint32_t y = 0; numBlockHorizontal > y;y++)
		{

			if (mapChipField_->GetMapChipTypeByIndex(y, x) == MapChipType::kBlock)
			{
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
	
				worldTransformBlocks_[x][y] = worldTransform;
				worldTransformBlocks_[x][y]->translation_ = mapChipField_->GetMapChipPositionByIndex(y, x);
			}
		}
	 }
}

bool GameScene::IsCollision(AABB aabb1, AABB aabb2) 
{
	bool ATFlag = false;

	// 当たってるか当たってないか判断するフラグ
	if (aabb1.max.y > aabb2.min.y && aabb1.min.y < aabb2.max.y) {

		if (aabb1.max.x > aabb2.min.x && aabb1.min.x < aabb2.max.x) {
			ATFlag = true;
		}
	} else {
		ATFlag = false;
	}

	return ATFlag;

}
