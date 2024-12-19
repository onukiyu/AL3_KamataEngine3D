#include "Enemy.h"
#include <cassert>
//#include "TextureManager.h"
#include "MathUtilityForText.h"
#include"Player.h"

Enemy::~Enemy() {
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, uint32_t textureHandle) {

	// NULLポインタチェック
	assert(model);

	// 引数として受け取ったデータをメンバ変数に記録する
	model_ = model;
	textureHandle_ = textureHandle;
	// viewProjection_ = viewProjection;

	// ワールド変換の初期化
	worldTransform_.Initialize();
	
	worldTransform_.translation_ = {2.0f, 2.0f, 40.0f};
	
	//発射関数（かり）
	//Fire();

	// 接近フェーズ初期化
	ApproachInitialize();
}

void Enemy::Update() {

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	switch (phase_) {
	case Enemy::Phase::Approach:
	default:

		ApproachUpdate();

		break;
	case Enemy::Phase::Leave:
		// 移動（ベクトルを加算）
		worldTransform_.translation_ -= velocity_;
		break;
	}

	
	worldTransform_.UpdateMatrix();

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Enemy::Draw(ViewProjection& viewProjection) {

	// 3Dモデルを描画
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

void Enemy::Fire() {
	assert(player_);

	// 弾の速度
	//const float kBulletSpeed = 1.0f;

	// 自キャラのワールド座標を取得する
	Vector3 playerPos = player_->GetWorldPosition();
	// 敵キャラのワールド座標を取得する
	Vector3 enemyPos = GetWorldPosition();
	// 敵キャラから自キャラへの差分ベクトルを求める
	Vector3 differenceVector = enemyPos - playerPos;
	// ベクトルの正規化
	Vector3 vectorLength = Normalize(differenceVector);
	// ベクトルの長さを、速さに合わせる
	Vector3 velocity = vectorLength;

	//Vector3 velocity(0, 0, kBulletSpeed);

	//// 速度ベクトルを自機の向きに合わせて回転させる
	//velocity = TransformNormal(velocity, worldTransform_.matWorld_);

	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);

	// 弾を登録する
	// bullet_ = newBullet;
	bullets_.push_back(newBullet);
}

void Enemy::ApproachInitialize() {
	// 発射タイマーを初期化
	fireTimer_ = 60;
}

void Enemy::ApproachUpdate() {

	// 移動(ベクトルを加算)
	worldTransform_.translation_ += velocity_;
	// 規定の位置に到達したら離脱
	if (worldTransform_.translation_.z < 0.0f) {
		phase_ = Enemy::Phase::Leave;
	}

	// 発射タイマーカウントダウン
	fireTimer_ -= 1;
	// 指定時間に達した
	if (fireTimer_ <= 0) {
		// 弾を発射
		Fire();
		// 発射タイマーを初期化
		fireTimer_ = kFireIniterval;
	}
}

Vector3 Enemy::GetWorldPosition() { 
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}
