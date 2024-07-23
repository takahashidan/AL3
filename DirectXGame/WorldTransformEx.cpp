#include "WorldTransform.h"
#include "Matrix4x4Function.h"

void WorldTransform::UpdateMatrix() 
{
	matWorld_ = Matrix4x4Function::MakeAffineMatrix(scale_, rotation_, translation_);

	WorldTransform::TransferMatrix();
}