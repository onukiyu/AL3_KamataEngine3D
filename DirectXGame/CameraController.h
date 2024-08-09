#pragma once
#include "ViewProjection.h"
#include "MathUtilityForText.h"
///</snmmary>
///カメラコントローラ
///</summary>

//前方宣言
class Player;

class CameraController {
public:

	//矩形
	struct Rect {
		float left = 0.0f;  //左端
		float right = 1.0f; //右端
		float bottom = 0.0f;//下端
		float top = 1.0f;   //上端
	};

	///</snmmary>
	/// 初期化
	///</summary>
	void Initialize();
	
	///</snmmary>
	/// 更新
	///</summary>
	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();


	//追加
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	void SetMovableArea(Rect area) { movableArea_ = area; }

private:
	//ビュープロジェクション
	ViewProjection viewProjection_;

	Player* target_ = nullptr;

	//追従対称とカメラの座標の差（オフセット）
	Vector3 targetoffset_ = {0, 0, -15.0f};

	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 100};

	//カメラの目標座標
	Vector3 cameraTarget_;

	//座標補間割合
	static inline const float kInterpolationRate = 0.1f;

	//速度掛け算
	static inline const float kVelocityBias = 30.0f;

	static inline const Rect targetMargin = {-9.0f, 9.0f, -5.0f, 5.0f};
};


