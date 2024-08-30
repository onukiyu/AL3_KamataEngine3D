#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Player.h"
#include <vector>
#include "DebugCamera.h"
#include "Skydome.h"
#include "MapChipField.h"
#include "CameraController.h"
#include "Enemy.h"
#include "DeathParticles.h"


/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	// デスフラグのgetter
	bool IsFinished() const { return finished_; }

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	//自キャラ
	Player* player_ = nullptr;


	//Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;


	/// <summary>
	/// ゲームシーン用
	/// </summary>
	
	//テクスチャハンドル
	//uint32_t textureHandle_ = 0;

	//3Dモデルデータ
	Model* model_ = nullptr;

	Model* modelEnemy_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	//ブロック3Dモデルデータ
	Model* modelBlock_ = nullptr;

	//可変個配列
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;

	//デバッグカメラ有効
	bool isDebugCameraActive_ = false;

	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;

	//天球
	Skydome* skydome_ = nullptr;

	//3Dモデル
	Model* modelSkydome_ = nullptr;

	//マップチップフィールド
	MapChipField* mapChipField_ = nullptr;

	CameraController* cameraController_ = nullptr;

	void GenerateBlocks();

	void CheckAllColisions();

	//void IsFinished();

	DeathParticles* deathParticles_ = nullptr;
	Model* modelDeathParticles_ = nullptr;

	//ゲームのフェーズ（型）
	enum class Phase {
		kPlay,  //ゲームプレイ
		kDeath, //デス演出
	};

	//ゲームの現在フェーズ（変数）
	 Phase phase_;

	 void ChangePhase();

	 // 終了フラグ
	 bool finished_ = false;
};
