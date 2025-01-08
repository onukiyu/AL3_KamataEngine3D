#include "Audio.h"
#include "AxisIndicator.h"
#include "DirectXCommon.h"
#include "GameScene.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include "WinApp.h"
#include "TitleScene.h"
#include "ClearScene.h"
#include "DeathScene.h"


TitleScene* titleScene = nullptr;
GameScene* gameScene = nullptr;
ClearScene* clearScene = nullptr;
DeathScene* deathScene = nullptr;

enum class Scene {
	kUnknown = 0,

	kTitle,
	kGame,
	kClear,
	kDeath
};

Scene scene = Scene::kUnknown;

// シーンの切り替え
void ChangeScene();
// シーンの更新
void UpdateScene();
// シーンの描画
void DrawScene();

// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	WinApp* win = nullptr;
	DirectXCommon* dxCommon = nullptr;
	// 汎用機能
	Input* input = nullptr;
	Audio* audio = nullptr;
	AxisIndicator* axisIndicator = nullptr;
	PrimitiveDrawer* primitiveDrawer = nullptr;
	//GameScene* gameScene = nullptr;

	// ゲームウィンドウの作成
	win = WinApp::GetInstance();
	win->CreateGameWindow(L"LE2D_04_オオヌキ_ユウ_冬の3Dゲーム");

	// DirectX初期化処理
	dxCommon = DirectXCommon::GetInstance();
	dxCommon->Initialize(win);

#pragma region 汎用機能初期化
	// ImGuiの初期化
	ImGuiManager* imguiManager = ImGuiManager::GetInstance();
	imguiManager->Initialize(win, dxCommon);

	// 入力の初期化
	input = Input::GetInstance();
	input->Initialize();

	// オーディオの初期化
	audio = Audio::GetInstance();
	audio->Initialize();

	// テクスチャマネージャの初期化
	TextureManager::GetInstance()->Initialize(dxCommon->GetDevice());
	TextureManager::Load("white1x1.png");

	// スプライト静的初期化
	Sprite::StaticInitialize(dxCommon->GetDevice(), WinApp::kWindowWidth, WinApp::kWindowHeight);

	// 3Dモデル静的初期化
	Model::StaticInitialize();

	// 軸方向表示初期化
	axisIndicator = AxisIndicator::GetInstance();
	axisIndicator->Initialize();

	primitiveDrawer = PrimitiveDrawer::GetInstance();
	primitiveDrawer->Initialize();
#pragma endregion

	// ゲームシーンの初期化
	/*gameScene = new GameScene();
	gameScene->Initialize();*/

	// タイトルシーンの初期化
	scene = Scene::kTitle;
	titleScene = new TitleScene;
	titleScene->Initialize();

	// メインループ
	while (true) {
		// メッセージ処理
		if (win->ProcessMessage()) {
			break;
		}

		// ImGui受付開始
		imguiManager->Begin();
		// 入力関連の毎フレーム処理
		input->Update();
		// ゲームシーンの毎フレーム処理
		//gameScene->Update();
		ChangeScene();
		UpdateScene();
		// 軸表示の更新
		axisIndicator->Update();
		// ImGui受付終了
		imguiManager->End();

		// 描画開始
		dxCommon->PreDraw();
		// ゲームシーンの描画
		//gameScene->Draw();
		DrawScene();


#ifdef _DEBUG
	// 軸表示の描画
		axisIndicator->Draw();
#endif // DEBUG

		// プリミティブ描画のリセット
		primitiveDrawer->Reset();
		// ImGui描画
		imguiManager->Draw();
		// 描画終了
		dxCommon->PostDraw();
	}

	// 各種解放
	delete titleScene;
	delete gameScene;
	delete clearScene;
	delete deathScene;
	// 3Dモデル解放
	Model::StaticFinalize();
	audio->Finalize();
	// ImGui解放
	imguiManager->Finalize();

	// ゲームウィンドウの破棄
	win->TerminateGameWindow();

	return 0;
}

void ChangeScene() {
	switch (scene) {
	case Scene::kTitle:
		if (titleScene->finish()) {
			// シーンを次のシーンへ
			scene = Scene::kGame;
			// 古いシーンを解放
			delete titleScene;
			titleScene = nullptr;
			// 次のシーンを初期化
			gameScene = new GameScene;
			gameScene->Initialize();
		}
		break;
	case Scene::kGame:
		if (gameScene->clear()) {
			// シーンを次のシーンへ
			scene = Scene::kClear;
			// 古いシーンを解放
			delete gameScene;
			gameScene = nullptr;
			// 次のシーンを初期化
			clearScene = new ClearScene;
			clearScene->Initialize();
		} else if (gameScene->finish()) {
			// シーンを次のシーンへ
			scene = Scene::kDeath;
			// 古いシーンを解放
			delete gameScene;
			gameScene = nullptr;
			// 次のシーンを初期化
			deathScene = new DeathScene;
			deathScene->Initialize();
		}
		break;
	case Scene::kClear:
		if (clearScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの変更
			delete clearScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	case Scene::kDeath:
		if (deathScene->IsFinished()) {
			// シーン変更
			scene = Scene::kTitle;
			// 旧シーンの変更
			// delete deathScene;
			titleScene = nullptr;
			// 新シーンの生成と初期化
			titleScene = new TitleScene;
			titleScene->Initialize();
		}
		break;
	}
}

void UpdateScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Update();
		break;
	case Scene::kGame:
		gameScene->Update();
		break;
	case Scene::kClear:
		clearScene->Update();
		break;
	case Scene::kDeath:
		deathScene->Update();
		break;
	}
}

void DrawScene() {
	switch (scene) {
	case Scene::kTitle:
		titleScene->Draw();
		break;
	case Scene::kGame:
		gameScene->Draw();
		break;
	case Scene::kClear:
		clearScene->Draw();
		break;
	case Scene::kDeath:
		deathScene->Draw();
		break;
	}
}