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
			// シーン変更
			scene = Scene::kGame;
			// 旧Sceneの解放
			delete titleScene;
			titleScene = nullptr;
			// 新Sceneの生成と初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->IsFinished())
		{
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの解放
			delete gameScene;
			gameScene = nullptr;
			// 新シーンの生成と初期化
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
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	
	


	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"GC2C_06_タカハシ_ダン");

	// DirectX初期化処理
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

			// ImGui受付開始
			imguiManager->Begin();
			// 入力関連の毎フレーム処理
			input->Update();
			// ゲームシーンの毎フレーム処理
			//gameScene->Update();
			// 軸表示の更新
			axisIndicator->Update();
			// タイトルの更新
			//titleScene->Update(keys, preKeys);
			// 現在Sceneの更新
			UpdateScene();
			//   シーン切り替え
			ChangeScene();
			// ImGui受付終了
			imguiManager->End();

			// 描画開始
			dxCommon->PreDraw();
			// タイトルの描画
			// titleScene->Draw();
			//   現在シーンの描画
			DrawScene();;
			// 軸表示の描画
			axisIndicator->Draw();
			// プリミティブ描画のリセット
			primitiveDrawer->Reset();
			// ImGui描画
			imguiManager->Draw();
			// 描画終了
			dxCommon->PostDraw();

			break;
		case Scene::kGame:

			// ImGui受付開始
			imguiManager->Begin();
			// 入力関連の毎フレーム処理
			input->Update();
			// ゲームシーンの毎フレーム処理
			gameScene->Update();
			// 軸表示の更新
			axisIndicator->Update();
			// タイトルの更新
			// titleScene->Update(keys, preKeys);
			// 現在Sceneの更新
			UpdateScene();
			// シーン切り替え
			ChangeScene();
			// ImGui受付終了
			imguiManager->End();

			// 描画開始
			dxCommon->PreDraw();
			// タイトルの描画
			// titleScene->Draw();
			// 現在シーンの描画
			DrawScene();
			// ゲームシーンの描画
			//gameScene->Draw();
			// 軸表示の描画
			axisIndicator->Draw();
			// プリミティブ描画のリセット
			primitiveDrawer->Reset();
			// ImGui描画
			imguiManager->Draw();
			// 描画終了
			dxCommon->PostDraw();

			break;
		}

		
	}

	// 各種解放
	delete gameScene;
	delete titleScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	
	

	return 0;
}


