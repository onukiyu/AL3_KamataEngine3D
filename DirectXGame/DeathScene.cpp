#include "DeathScene.h"

void DeathScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	DeathWorldTransform_.Initialize();
	DeathViewProjection_.Initialize();

	DeathHandle_ = TextureManager::Load("DeathScene.png");

	deathSprite_ = Sprite::Create(DeathHandle_, {0, 0});

	audio_ = Audio::GetInstance();

	// サウンドデータ読み込み
	soundDataHandle_ = audio_->LoadWave("GameOver.mp3");
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void DeathScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_R)) {

		audio_->StopWave(voiceHandle_);

		finished_ = true;
	}
}

void DeathScene::Draw() {
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	deathSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

	Model::PreDraw(commandList);

	Model::PostDraw();
}