#include "TitleScene.h"
#include <numbers>

TitleScene::TitleScene() 
{
}

TitleScene::~TitleScene() 
{
	delete modelFont_;
	delete modelPlayer_;
}

void TitleScene::Initialize() 
{
	modelFont_ = Model::CreateFromOBJ("titleFont");
	modelPlayer_ = Model::CreateFromOBJ("player");

	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	worldTransformFont_.Initialize();
	worldTransformPlayer_.Initialize();

	dxCommon_ = DirectXCommon::GetInstance();

	worldTransformFont_.translation_.y = 10;
	worldTransformPlayer_.translation_.y = -8;
	worldTransformFont_.scale_ = {2, 2, 2};
	worldTransformPlayer_.scale_ = {10, 10, 10};
	worldTransformPlayer_.rotation_.y = std::numbers::pi_v<float>;
}

void TitleScene::Update()
{
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}

	//行列の更新
	worldTransformFont_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

void TitleScene::Draw()
{
	//コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	//3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	///ここに3Dオブジェクト描画処理を追加できる
	modelFont_->Draw(worldTransformFont_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);


	//3Dオブジェクト描画後処理
	Model::PostDraw();
}


