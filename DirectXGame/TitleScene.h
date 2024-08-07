#pragma once
#include "Input.h"
#include "Player.h"

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

};
