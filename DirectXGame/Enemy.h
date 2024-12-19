#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "EnemyBullet.h"
#include <list>

// 自機クラスの前方宣言
class Player;

class Enemy {
public:
	~Enemy();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Fire();



	// 発射感覚
	static const int kFireIniterval = 60;

	// 接近フェーズ初期化
	void ApproachInitialize();

	// 接近フェーズ更新
	void ApproachUpdate();

	// 自キャラ
	Player* player_ = nullptr;

	void SetPlayer(Player* player) { player_ = player; }

	// ワールド座標を取得
	Vector3 GetWorldPosition();


	void OnCollision();

	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	// テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// 速度
	Vector3 velocity_ = {0.02f, 0.0f, -0.02f};

	// 行動フェーズ
	enum class Phase {
		Approach, //接近する
		Leave,    //離脱する
	};

	// フェーズ
	Phase phase_ = Phase::Approach;

	// 弾
	//EnemyBullet* bullet_ = nullptr;
	std::list<EnemyBullet*> bullets_;

	// 発射タイマー
	int32_t fireTimer_ = 0;
};
