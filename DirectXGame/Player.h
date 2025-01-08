#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "Input.h"
#include "PlayerBullet.h"
#include <list>

class Player {

public:

	~Player();

	void Initialize(Model* model, uint32_t textureHandle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	void Attack();
	
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	void OnCollision();

	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }

	int Gethitpoint() { return playerHp; }

private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	// ワールド変換データ
	WorldTransform worldTransform2_;

	//キーボード入力
	Input* input_ = nullptr;

	//弾
	//PlayerBullet* bullet_ = nullptr;
	std::list<PlayerBullet*> bullets_;

	// 体力
	int playerHp = 2;

	
	
};
