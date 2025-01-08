#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "TextureManager.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class TitleScene {
public:
	void Initialize();

	void Update();

	void Draw();

	bool finish() const { return isfinished_; }

private:
	bool isfinished_ = false;

	DirectXCommon* dxCommon_ = nullptr;

	ViewProjection titleViewProjection_;

	WorldTransform titleWorldTransform_;

	Sprite* titleSprite_ = nullptr;

	uint32_t titleHandle_;

	Audio* audio_ = nullptr;

	// サウンドデータハンドル
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;
};