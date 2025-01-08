#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class DeathScene {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	bool IsFinished() const { return finished_; }

private:
	bool finished_ = false;

	DirectXCommon* dxCommon_ = nullptr;
	WorldTransform DeathWorldTransform_;
	ViewProjection DeathViewProjection_;
	uint32_t DeathHandle_;

	// スプライト
	Sprite* deathSprite_ = nullptr;

	Audio* audio_ = nullptr;

	// サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;
};
