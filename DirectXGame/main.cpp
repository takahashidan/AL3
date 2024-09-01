#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"




enum class Scene
{
	kUnkown = 0,

		kTitle,
		kGame,
};

Scene scene = Scene::kUnkown;
TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
Model* model_ = nullptr;
WorldTransform* wolrldTransform_ = {};
ViewProjection* viewProjection_ = {}; 
MapChipField* mapChipField_ = nullptr;

    // キー入力結果を受け取る箱
char keys[256] = {0};
char preKeys[256] = {0};

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->IsFinished()) {
			scene = Scene::kGame;
			delete titleScene;
			titleScene = nullptr;
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished())
		{
			scene = Scene::kTitle;
			delete gameScene;
			gameScene = nullptr;
			titleScene = new TitleScene();
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() 
{
	switch (scene) 
	{
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	}
}

void DrawScene() 
{
	switch (scene) 
	{
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	}
}

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"expedition");

	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	gameScene = new GameScene();
	gameScene->Initialize();

	scene = Scene::kTitle;

	titleScene = new TitleScene;
	titleScene->Initialize();

	


	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		switch (scene) {
		case Scene::kTitle:

			imguiManager->Begin();
			input->Update();
			axisIndicator->Update();
			UpdateScene();
			ChangeScene();
			imguiManager->End();

			dxCommon->PreDraw();
			DrawScene();;
			axisIndicator->Draw();
			primitiveDrawer->Reset();
			imguiManager->Draw();
			dxCommon->PostDraw();

			break;
		case Scene::kGame:

			imguiManager->Begin();
			input->Update();
			gameScene->Update();
			axisIndicator->Update();
			UpdateScene();
			ChangeScene();
			imguiManager->End();

			dxCommon->PreDraw();
			DrawScene();
			axisIndicator->Draw();
			primitiveDrawer->Reset();
			imguiManager->Draw();
			dxCommon->PostDraw();

			break;
		}

		
	}

	delete gameScene;
	delete titleScene;
	Model::StaticFinalize();
	audio->Finalize();
	imguiManager->Finalize();

	win->TerminateGameWindow();

	
	

	return 0;
}


