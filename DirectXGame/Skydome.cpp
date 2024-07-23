#include "Skydome.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection) { 
	worldTransform_.Initialize();
	
	model_ = model;


	viewProjection_ = viewProjection;

	//model_ = Model::Create();
	//worldTransform_ = new WorldTransform; 
	//viewProjection_ = new ViewProjection;
	
}

Skydome::Skydome() {}

Skydome::~Skydome() {}



void Skydome::Update() {}

void Skydome::Draw()
{
	model_->Draw(worldTransform_, *viewProjection_);
}


