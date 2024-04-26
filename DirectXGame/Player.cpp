#include "Player.h"
#include "TextureManager.h"
#include <cassert>
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "AxisIndicator.h"
void Player::Initialize(Model* model, uint32_t textureHandle, ViewProjection* viewProjection) {

	// NULLポインタチェック
	assert(model);
	//引数として受け取ったデータをメンバ変数に記録する
	//ローカル変数は関数が終わると消える
	model_ = model;
	textureHandle_ = textureHandle;
	viewProjection_ = viewProjection;
	//ワールド変換の初期化
	worldTransform_.Initialize();
}

void Player::Update() {

	//行列を定数バッファに転送
	worldTransform_.TransferMatrix();
}

void Player::Draw() {

	//3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_, textureHandle_);
}


