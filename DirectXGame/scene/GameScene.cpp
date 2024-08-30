#include "GameScene.h"
#include "TextureManager.h"
#include "MathUtilityForText.h"
#include "MapChipField.h"
#include "CameraController.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() { 
	delete model_;
	//自キャラの解放
	delete player_;

	//ブロック3Dモデルデータの解放
	delete modelBlock_;

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	delete debugCamera_;

	delete skydome_;

	delete modelSkydome_;
	//マップチップフィールドの解放
	delete mapChipField_;

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	

	delete modelEnemy_;

	delete modelDeathParticles_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//ファイル名を指定してテクスチャを読み込む
	//textureHandle_ = TextureManager::Load("player.png");

	//3Dモデルデータの生成
	model_ = Model::CreateFromOBJ("player");

	modelEnemy_ = Model::CreateFromOBJ("enemy");

	//座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(2, 19); 

	//Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12, 19); 

	//ビュープロジェクションの初期化
	viewProjection_.Initialize();

	

	//ブロック3Dモデルデータの生成
	modelBlock_ = Model::CreateFromOBJ("block");

	//要素数
	//const uint32_t kNumBlockVirtical = 10;//縦
	//const uint32_t kNumBlockHorizontal = 20;//横
	//ブロック1個分の横幅
	/*const float kBlockWidth = 2.0f;
	const float kBlockHeight = 2.0f;*/

	//要素数を変換する
	//列数を設定（縦方向のブロック数）
	/*worldTransformBlocks_.resize(kNumBlockVirtical);*/

	
	//for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//	//1列の要素数を設定（横方向のブロック数）
	//	worldTransformBlocks_[i].resize(kNumBlockHorizontal);
	//}

	////キューブの生成
	//for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
	//	for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
	//		if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
	//			WorldTransform* worldTransform = new WorldTransform();
	//			worldTransform->Initialize();
	//			worldTransformBlocks_[i][j] = worldTransform;
	//			worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
	//		}
			//if((i + j) % 2 == 0) continue;//市松模様

			//worldTransformBlocks_[i][j] = new WorldTransform();
			//worldTransformBlocks_[i][j]->Initialize();
			//worldTransformBlocks_[i][j]->translation_.x = kBlockWidth * j;
			//worldTransformBlocks_[i][j]->translation_.y = kBlockHeight * i;
		/*}
	}*/

	//デバッグカメラの生成
	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	
	//3Dモデルの生成
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	
	//天球の生成
	skydome_ = new Skydome();
	
	//天球の初期化
	skydome_->Initialize(modelSkydome_, &viewProjection_);


	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");

	GenerateBlocks();//23,24ページ
	
	// 自キャラの生成
	player_ = new Player();
	//自キャラの初期化
	playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);
	player_->Initialize(model_,&viewProjection_, playerPosition);

	player_->SetMapChipField(mapChipField_);

	for (int32_t i = 0; i < 3; ++i) {
		Enemy* newEnemy_ = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(11 + i * 3, 18 - i);
		newEnemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy_);
	}

	cameraController_ = new CameraController();
	cameraController_->Initialize();
	
	cameraController_->SetTarget(player_);
	cameraController_->Reset();

	CameraController::Rect cameraArea = {12.0f, 100 - 12.0f, 6.0f, 6.0f};

	cameraController_->SetMovableArea(cameraArea);

	/*player_->Update();
	Vector3 position = player_->GetWorldPosition();*/


	modelDeathParticles_= Model::CreateFromOBJ("deathParticle");
	

	//ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::Update() {
	
	switch (phase_) {
	case Phase::kPlay:
		//ゲームプレイフェーズの処理
		
		//天球の更新
		skydome_->Update();

		// 自キャラの更新
		player_->Update();

		//敵の更新（複数）
		for (Enemy* enemy : enemies_) {
			 enemy->Update();
		}

		//カメラコントローラの更新
		cameraController_->Update();

		// デバッグカメラの更新
		debugCamera_->Update();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;
				// 平行移動
				/*Matrix4x4 result{
				    1.0f, 0.0f, 0.0f, 0.0f,
				    0.0f, 1.0f, 0.0f, 0.0f,
				    0.0f, 0.0f, 1.0f, 0.0f,
				    worldTransformBlock->translation_.x,
				    worldTransformBlock->translation_.y,
				    worldTransformBlock->translation_.z,
				    1.0f};*/

				// Matrix4x4 matWorld = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_);

				// worldTransformBlock->matWorld_ = matWorld;

				//// 平行移動だけ代入
				///*worldTransformBlock->matWorld_ = result;*/

				//// 定数バッファに転送する
				// worldTransformBlock->TransferMatrix();

				// アフィン変換と転送
				worldTransformBlock->UpdateMatrix();
			}
		}

		// 全ての当たり判定を行う
		CheckAllColisions();

		break;

	case Phase::kDeath:
		//デス演出フェーズの処理

		// 天球の更新
		skydome_->Update();

		// 敵の更新（複数）
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		//デスパーティクルの更新
		if (deathParticles_) {
			deathParticles_->Update();
		}

		// デバッグカメラの更新
		debugCamera_->Update();

		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				// アフィン変換と転送
				worldTransformBlock->UpdateMatrix();
			}
		}

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}

		break;
	}
	

	

	

	

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_0)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif // _DEBUG

	//カメラの処理
	if (isDebugCameraActive_) {
		//デバッグカメラの更新
		debugCamera_->Update();
		//デバッグカメラのビュー行列
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		//デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の転送
		viewProjection_.TransferMatrix();
	} else {
		// デバッグカメラのビュー行列
		viewProjection_.matView = cameraController_->GetViewProjection().matView;
		// デバッグカメラのプロジェクション行列
		viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
		//ビュープロジェクション行列の更新と転送
		viewProjection_.TransferMatrix();
	}

	
	ChangePhase();
	

	
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

	//自キャラの描画
	player_->Draw();
	
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}

	skydome_->Draw();

	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine){
			if (!worldTransformBlock)
				continue;
			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	if (deathParticles_) {
		deathParticles_->Draw();
	}
	

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
void GameScene::GenerateBlocks() {
	// 要素数
	uint32_t numBlockVirtical = mapChipField_->GetNumBlockVirtical();
	uint32_t numBlockHorizontal = mapChipField_->GetNumBlockHorizontal();

	// 要素数を変更する
	// 列数を設定（縦方向のブロック数）
	worldTransformBlocks_.resize(numBlockVirtical);
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		// 1列の要素数を設定（横方向のブロック数）
		worldTransformBlocks_[i].resize(numBlockHorizontal);
	}
	for (uint32_t i = 0; i < numBlockVirtical; ++i) {
		for (uint32_t j = 0; j < numBlockHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformBlocks_[i][j] = worldTransform;
				worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

//自キャラと敵キャラの当たり判定
void GameScene::CheckAllColisions()
{
#pragma region 
	{
		//判定対称1と2の座標
		AABB aabb1, aabb2;

		//自キャラの座標
		aabb1 = player_->GetAABB();

		//自キャラと敵弾全ての当たり判定
		for (Enemy* enemy : enemies_) {
			//敵弾の座標
			aabb2 = enemy->GetAABB();

			//AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				//自キャラの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				//敵弾の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}

		}
	}


}

void GameScene::ChangePhase() {
	switch (phase_) {
	case GameScene::Phase::kPlay:

		if (player_->IsDead() == true) {
			//死亡演出
			phase_ = Phase::kDeath;
			
			const Vector3& deathParticlesPosition = player_->GetWorldPosition();
		
			deathParticles_ = new DeathParticles;
			deathParticles_->Initialize(modelDeathParticles_, &viewProjection_, deathParticlesPosition);
		}
		//
		


			break;
	case GameScene::Phase::kDeath:


		break;
	}
}




