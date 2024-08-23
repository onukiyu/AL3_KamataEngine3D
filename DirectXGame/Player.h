#pragma once
#include "Model.h"
#include "WorldTransform.h"
#include "MathUtilityForText.h"
#include "ViewProjection.h"

class MapChipField;

class Enemy;

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

	

	//加速
	static inline const float kAcceleration = (float)0.01;
	//減速
	static inline const float kAttenuation = (float)0.02;
	//最大速度
	static inline const float kLimitRunSpeed = (float)1.0f;

	//着地時の速度減衰率
	static inline const float kAttenuationWall = 0.1f;

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


	//追加
	const WorldTransform& GetWorldTransform() const { return worldTransform_; }

	const Vector3& GetVelocity() const { return velocity_; }

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	//旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	//旋回タイマー
	float turnTimer_ = 0.0f;

	//旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;


	//重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.05f;
	//重力落下速度（下方向）
	static inline const float kLimitFallSpeed = 0.5f;
	//ジャンプ速度（上方向）
	static inline const float kJumpAcceleration = 0.5f;

	//接地状態フラグ
	bool onGround_ = true;

	//着地時の速度減衰率
	static inline const float kAttenuationLanding = 0.1f;

	

	//落下用
	static inline const float kGroundSearchHeight = 0.06f;
	
	
	// ワールド座標を取得
	Vector3 GetWorldPosition();

	//AABBを取得
	AABB GetAABB();

	// 衝突応用
	void OnCollision(const Enemy* enemy);

	private: // メンバ変数

	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
	//テクスチャハンドル
	uint32_t textureHandle_ = 0u;

	ViewProjection* viewProjection_ = nullptr;
	
	//マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;

	//キャラクターの当たり判定サイズ
	static inline const float kWidth = 0.8f;
	static inline const float kHeight = 0.8f;

	//1.移動入力
	void InputMove();

	//2.マップ衝突判定
	struct CollisionMapInfo {
		bool ceiling = false;
		bool landing = false;
		bool hitwall = false;
		Vector3 move;
	};

	//角
	enum Corner {
		kRightBottom, //右下
		kLeftBottom, //左下
		kRightTop,    //右上
		kLeftTop,     //左上

		kNumCorner    //要素数
	};

	void CheckMapCollision(CollisionMapInfo& info);
	void CheckMapCollisionUp(CollisionMapInfo& info);

	void CheckMapCollisionDown(CollisionMapInfo& info);
	void CheckMapCollisionRight(CollisionMapInfo& info);
	void CheckMapCollisionLeft(CollisionMapInfo& info);

	//3.判定結果を反映して移動させる
	void CheckMapMove(CollisionMapInfo& info);

	//4.天井に接触している場合の処理
	void CheckMapCeiling(CollisionMapInfo& info);

	Vector3 CornerPosition(const Vector3& center, Corner corner);
	static inline const float kBlank = 0.04f;
	//7.旋回制御
	void AnimateTurn();

	//6.接地状態の切り替え処理
	void CheckMapLanding(const CollisionMapInfo& info);

	//5.壁に接触している場合の処理
	void CheckMapWall(const CollisionMapInfo& info);

	

};
