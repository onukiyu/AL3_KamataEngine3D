#include "ClearScene.h"

void ClearScene::Initialize() {
	dxCommon_ = DirectXCommon::GetInstance();
	ClearWorldTransform_.Initialize();
	ClearViewProjection_.Initialize();

	ClearHandle_ = TextureManager::Load("ClearScene.png");

	clearSprite_ = Sprite::Create(ClearHandle_, {0, 0});

	audio_ = Audio::GetInstance();

	// サウンドデータ読み込み
	soundDataHandle_ = audio_->LoadWave("GameClear.mp3");
	// 音声再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void ClearScene::Update() {
	if (Input::GetInstance()->TriggerKey(DIK_R)) {

		audio_->StopWave(voiceHandle_);

		finished_ = true;
	}
}

void ClearScene::Draw() {

	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	clearSprite_->Draw();

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

	Model::PreDraw(commandList);

	Model::PostDraw();
}