#include "GameScene.h"
#include "TextureManager.h"
//#include "Player.h"/
#include <cassert>
#include "AxisIndicator.h"
#include "MathUtilityForText.h"

GameScene::GameScene() {}

GameScene::~GameScene() {

	// 3Dモデルデータの解放
	delete model_;

	// 自キャラの解放
	delete player_;

	delete debugCamera_;

	delete enemy_;

	delete skyDome_;

	delete modelSkydome_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	// ファイル名を指定してテクスチャを読み込む
	textureHandle_ = TextureManager::Load("mario.jpg");

	// 3Dモデルデータの生成
	model_ = Model::Create();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 1000; // ビュープロジェクションのfarZを適度に大きい値に変更する
	viewProjection_.Initialize();

	// 自キャラの生成
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(model_, textureHandle_);

	// 敵の生成
	enemy_ = new Enemy();
	// 敵の初期化
	enemy_->Initialize(model_, textureHandle_);


	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	//軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	//軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);

	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);

	// 天球の生成
	skyDome_ = new Skydome();
	
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	// 天球の初期化
	skyDome_->Initialize(modelSkydome_);

	//audio_ = Audio::GetInstance();

	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("integrate.wav");
	//音声再生
	voiceHandle_= audio_->PlayWave(soundDataHandle_, true);
}

void GameScene::Update() {
	// 自キャラの更新
	player_->Update();

	// 敵の更新
	enemy_->Update();

	// デバッグカメラの更新
	debugCamera_->Update();

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	// カメラの処理
	if (isDebugCameraActive_) {
		// デバッグカメラの更新
		debugCamera_->Update();
		// デバッグカメラのビュー行列
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		// デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// デバッグカメラのビュー行列
		//viewProjection_.matView = cameraController_->GetViewProjection().matView;
		// デバッグカメラのプロジェクション行列
		//viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}
	CheckAllCollisions();

	// 天球の更新
	skyDome_->Update();

	//audio_->StopWave(voiceHandle_);

	if (player_->Gethitpoint() <= 0) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
		isfinished_ = true;
	}

	if (enemy_->Gethitpoint() <= 0) {
		// 音声停止
		audio_->StopWave(voiceHandle_);
		iscleared_ = true;
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// 自キャラの描画
	player_->Draw(viewProjection_);

	//敵の描画
	enemy_->Draw(viewProjection_);

	// 天球の描画
	skyDome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheckAllCollisions() {
	// 判定対称AとBの座標
	Vector3 posA, posB;


	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();

	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

	#pragma region 
	// 自キャラの座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾全ての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標の差分ベクトル
		Vector3 subtract = posB - posA;
		// 座標AとBの距離を求める
		float distance = Length(subtract);

		// 球と球の交差判定
		if (distance < 1.5f + 1.5f) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region
	// 敵キャラの座標
	posA = enemy_->GetWorldPosition();

	// 敵キャラと自弾全ての当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();

		// 座標の差分ベクトル
		Vector3 subtract = posB - posA;
		// 座標AとBの距離を求める
		float distance = Length(subtract);

		// 球と球の交差判定
		if (distance < 1.5f + 1.5f) {
			// 敵キャラの衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 自弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
	#pragma endregion

	#pragma region
	// 敵の座標
	for (EnemyBullet* enemyBullet : enemyBullets) {
		posA = enemyBullet->GetWorldPosition();

		// 敵キャラと自弾全ての当たり判定
		for (PlayerBullet* playerBullet : playerBullets) {
			// 敵弾の座標
			posB = playerBullet->GetWorldPosition();

			// 座標の差分ベクトル
			Vector3 subtract = posB - posA;
			// 座標AとBの距離を求める
			float distance = Length(subtract);

			// 球と球の交差判定
			if (distance < 1.5f + 1.5f) {
				// 敵キャラの衝突時コールバックを呼び出す
				enemyBullet->OnCollision();
				// 自弾の衝突時コールバックを呼び出す
				playerBullet->OnCollision();
			}
		}
	}
	#pragma endregion
}
