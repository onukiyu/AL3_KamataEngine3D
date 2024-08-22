#include "Enemy.h"
#include <cassert>
#include <numbers>

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) 
{
	//NULLポインタチェック
	assert(model);

	//引数として受け取ったデータをメンバ変数に記録する
	//ローカル変数は関数が終わると消える
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.translation_ = position;

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.rotation_.y = std::numbers::pi_v<float>/-2.0f;

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update()
{
	//移動
	worldTransform_.translation_ += velocity_;

	//タイマーを加算
	walkTimer_ += 1.0f / 60.0f;

	//アニメーション
	worldTransform_.rotation_.x = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer_ / kWalkMotionTime);

	//行列を更新
	worldTransform_.UpdateMatrix();

}

void Enemy::Draw()
{
	//3Dモデルを描画
	model_->Draw(worldTransform_, *viewProjection_);
}
