#include "Skydome.h"

void Skydome::Initialize(Model* model) {

	// ワールド変換の初期化
	worldTransform_.Initialize();

	model_ = model;

	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() {}

void Skydome::Draw(ViewProjection& viewProjection) {
	// 3Dモデル描画
	model_->Draw(worldTransform_, viewProjection);
}
