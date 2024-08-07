#include "TitleScene.h"
#include <dinput.h>

TitleScene::TitleScene() : isFinished_(false) {
}

TitleScene::~TitleScene() {
}

void TitleScene::Initialize() 
{
	isFinished_ = false;
}

void TitleScene::Update()
{

	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		isFinished_ = true;
	}
}

void TitleScene::Draw() 
{
	
}
