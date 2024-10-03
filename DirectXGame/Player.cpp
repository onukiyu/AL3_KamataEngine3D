#include "Player.h"
#include <cassert>

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();
	
}

void Player::Update() {
	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	
}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
