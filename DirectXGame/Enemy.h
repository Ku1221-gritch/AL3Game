#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "struct.h"

class MapChipField;
class Player;

class Enemy {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Enemy();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Enemy();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	///< summary>
	/// 描画
	/// </summary>
	void Draw();

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

	// 敵の動く速度
	static inline const float kWalkSpeed = 0.5f;

	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.05f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 0.3f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 0.3f;
	// 経過時間
	float walkTimer_ = 0.0f;
	// 速度
	Vector3 velocity_ = {};
	// ワールド座標を取得
	Vector3 GetWorldPosition();
	// AABBを取得
	AABB GetAABB();
	// プレイヤーとの衝突判定
	void OnCollision(const Player* player);

//マップチップとの当たり判定
void MapCollisionDetectionUp(CollisionMapInfo& info);
void MapCollisionDetectionDown(CollisionMapInfo& info);
void MapCollisionDetectionRight(CollisionMapInfo& info);
void MapCollisionDetectionLeft(CollisionMapInfo& info);
Vector3 CornerPosition(const Vector3& center, Corner corner);

// 壁に接触している場合の処理
void WallContactProcess(const CollisionMapInfo& info);

//接地状態の切り替え処理
void GroundingStateSwitching(const CollisionMapInfo& info);

// 判定結果を反映して移動させる
void MoveJudgmentResults(const CollisionMapInfo& info);


private:
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// カメラ
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 敵の当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;
	static inline const float kBlank = 0.1f;

	static inline const float kAttenuation = 0.3f;
	static inline const float kAttenuationLanding = 0.1f;
	static inline const float kAdsorptionLanding = 0.2f;


	// 接地状態フラグ
	bool onGround_ = false;
	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.1f;
	// 最大落下速度（下方向
	static inline const float kLimitFallSpeed = 0.7f;

};