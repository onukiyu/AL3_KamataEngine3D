#include "TitleScene.h"

void TitleScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	titleViewProjection_.Initialize();
	titleWorldTransform_.Initialize();
	audio_ = Audio::GetInstance();

	titleHandle_ = TextureManager::Load("TitleScene.png");

	titleSprite_ = Sprite::Create(titleHandle_, {0, 0});

	// サウンドデータ読み込み
	soundDataHandle_ = audio_->LoadWave("Opening.mp3");
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void TitleScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_RETURN)) {

		audio_->StopWave(voiceHandle_);

		isfinished_ = true;
	}

	titleWorldTransform_.UpdateMatrix();
}

void TitleScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Sprite::PreDraw(commandList);

	titleSprite_->Draw();

	Sprite::PostDraw();

	dxCommon_->ClearDepthBuffer();

	Model::PreDraw(commandList);

	Model::PostDraw();
}