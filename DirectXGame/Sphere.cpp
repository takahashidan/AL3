#include"Sphere.h"

void Sphere::Initialize() {
}

void Sphere::Update() {
}

void Sphere::Draw() { 
	model_->Draw(worldTransform_, viewProjection_);
	}