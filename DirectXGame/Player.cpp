#include "Player.h"
#include "MathUtilityForText.h"
#include "Vector3.h"
#include <cassert>
#include "imgui.h"
#include "imgui_impl_dx12.h"
#include "imgui_impl_win32.h"

void Player::Initialize(Model* model, uint32_t textureHandle) {

	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	//viewProjection_ = viewProjection;
	
	//ワールド変換の初期化
	worldTransform_.Initialize();
	
	//シングルインスタンスを取得する
	input_ = Input::GetInstance();
}

void Player::Update() {
	//アフィン変換行列の作成
	

	//行列を定数バッファに転送
	//worldTransform_.TransferMatrix();

	//キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	//キャラクターの移動速さ
	const float kCharacterSpeed = 0.2f;

	//押した方向で移動ベクトルを変更（左右）
	if (input_->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	//押した方向で移動ベクトルを変更（上下）
	if (input_->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	//座標移動（ベクトルの加算）
	worldTransform_.translation_ += move;

	worldTransform_.UpdateMatrix();

	//移動限界座標
	const float kMoveLimitX = 34;
	const float kMoveLimitY = 18;

	//範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	//回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.02f;

	//押した方向で移動ベクトルを変更
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y += kRotSpeed;
	}


	//キャラクターの座標を画面表示する処理
#ifdef _DEBUG

	ImGui::Begin("DATA");
	ImGui::InputFloat3("Player", &worldTransform_.translation_.x);
	ImGui::End();

#endif // DEBUG


}

void Player::Draw(ViewProjection& viewProjection) {
	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
}
