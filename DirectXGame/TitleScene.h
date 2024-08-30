#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DirectXCommon.h"

class TitleScene {
public:
	TitleScene();
	~TitleScene();
	void Initialize();
	void Update();
	void Draw();

	//デスフラグのgetter
	bool IsFinished() const { return finished_; }

private:

	//終了フラグ
	bool finished_ = false;

	Model* modelFont_ = nullptr;
	Model* modelPlayer_ = nullptr;

	//ビュープロジェクション
	ViewProjection viewProjection_;

	WorldTransform worldTransformFont_;
	WorldTransform worldTransformPlayer_;

	DirectXCommon* dxCommon_ = nullptr;

	float timer_ = 0.0f;
	static inline const float kMotionTime = 1.0f;

};
