#include "CameraController.h"
#include "Player.h"
void CameraController::Initialize() 
{
	//ビュープロジェクションの初期化
	viewProjection_.Initialize();


}

void CameraController::Update()
{
	//追従対称のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();

	const Vector3& targetVelocity = target_->GetVelocity();

	//追従対称とオフセットと追従対称の速度からカメラの目標座標を計算
	cameraTarget_ = targetWorldTransform.translation_ + targetoffset_ + targetVelocity * kVelocityBias;

	//座標補間によりゆったり追従
	viewProjection_.translation_ = Lerp(viewProjection_.translation_, cameraTarget_, kInterpolationRate);

	//追従対称が画面外に出ないように補正
	viewProjection_.translation_.x = max(viewProjection_.translation_.x, targetWorldTransform.translation_.x + targetMargin.left);
	viewProjection_.translation_.x = min(viewProjection_.translation_.x, targetWorldTransform.translation_.x + targetMargin.right);
	viewProjection_.translation_.y = max(viewProjection_.translation_.y, targetWorldTransform.translation_.y + targetMargin.bottom);
	viewProjection_.translation_.y = min(viewProjection_.translation_.y, targetWorldTransform.translation_.y + targetMargin.top);

	//移動範囲制限
	viewProjection_.translation_.x = max(viewProjection_.translation_.x, movableArea_.left);
	viewProjection_.translation_.x = min(viewProjection_.translation_.x, movableArea_.right);
	viewProjection_.translation_.y = max(viewProjection_.translation_.y, movableArea_.bottom);
	viewProjection_.translation_.y = min(viewProjection_.translation_.y, movableArea_.top);

	//行列を更新
	viewProjection_.UpdateMatrix();

	
}

void CameraController::Reset()
{
	//追従対称のワールドトランスフォームを参照
	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	//追従対称とオフセットからカメラの座標を計算
	viewProjection_.translation_ = targetWorldTransform.translation_ + targetoffset_;

}
