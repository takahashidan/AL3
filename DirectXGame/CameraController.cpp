#include "CameraController.h"
#include "Player.h"


void CameraController::Initialize() 
{
	viewProjection_.Initialize();
}


void CameraController::Update() 
{
	// 追跡と賞のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTrnsform();
	// 追跡対象とオフセットからカメラの目標座標を計算
	cameraTargetCoordinates .x = targetWorldTransform.translation_.x + targetOffset_.x;
	cameraTargetCoordinates .y = targetWorldTransform.translation_.y + targetOffset_.y;
	cameraTargetCoordinates .z = targetWorldTransform.translation_.z + targetOffset_.z;

	// 座標補間によりゆったりと追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, cameraTargetCoordinates, kInterpolationRate);

	// 移動範囲制限
	viewProjection_.translation_.x = max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = min(viewProjection_.translation_.x, movableArea_.rigth);
	viewProjection_.translation_.y = max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = min(viewProjection_.translation_.y, movableArea_.top);

	// 行列を更新する
	viewProjection_.UpdateMatrix();
	viewProjection_.TransferMatrix();
}

void CameraController::Reset() 
{
	// 追跡対象のワールドトランスフォームを参照
	const WorldTransform& targetWorldTrnsform = target_->GetWorldTrnsform();

	// 追跡対象とオフセットからカメラの座標を計算
	viewProjection_.translation_.x = targetWorldTrnsform.translation_.x + targetOffset_.x;
	viewProjection_.translation_.y = targetWorldTrnsform.translation_.y + targetOffset_.y;
	viewProjection_.translation_.z = targetWorldTrnsform.translation_.z + targetOffset_.z;
}