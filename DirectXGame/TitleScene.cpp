#include "TitleScene.h"
#include "CameraController.h"
#include "Enemy.h"
#include "GameScene.h"
#include "ObjectColor.h"
#include "Player.h"
#include "Skydome.h"
#include "TextureManager.h"
#include <AABB.h>
#include <cassert>
#include <dinput.h>
#include <numbers>

TitleScene::TitleScene() : isFinished_(false) {
}

TitleScene::~TitleScene() { delete player_; }



void TitleScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();

	model_ = Model::CreateFromOBJ("title", true);
	
	player_ = new Player();
	viewProjection_ = new ViewProjection();

	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);

	player_->Initialize(model_, viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);



	worldTransform_.Initialize();
	viewProjection_->Initialize();
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		isFinished_ = true;
	}
}

void TitleScene::Draw() 
{

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
		Sprite::PreDraw(commandList);

		Sprite::PostDraw();
		dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
		Model::PreDraw(commandList);


		
	    model_->Draw(worldTransform_, *viewProjection_,textureHandle_);

		Model::PostDraw();

#pragma endregion
}




