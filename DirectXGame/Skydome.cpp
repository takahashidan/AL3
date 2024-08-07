#include "Skydome.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	worldTransform_.Initialize();
	
	model_ = model;
	viewProjection_ = viewProjection;	
}

Skydome::Skydome() {}

Skydome::~Skydome() {}



void Skydome::Update() {}

void Skydome::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_);
}


