#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
class Player {
public:
	///<summary>
	///初期化
	///</summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	/// <param name="viewProjection">ビュープロジェクション</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	Vector3 velocity_ = {};

	static inline const float kAcceleration = (float)0.01;

	static inline const float kAttenuation = (float)0.05;

	static inline const float kLimitRunSpeed = (float)0.3;

	//左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	///< summary>
	/// 更新
	///</summary>
	void Update();

	///< summary>
	/// 描画
	///</summary>
	void Draw();

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	//接地状態フラグ
	bool onGround_ = true;

	//重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.2f;
	//重力落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.2f;
	//ジャンプ速度（上方向）
	static inline const float kJumpAcceleration = 1.5f;

	private : // メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;
	

};
