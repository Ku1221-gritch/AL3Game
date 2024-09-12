#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "struct.h"

class MapChipField;
class Enemy;
class Needle;
class Goal;

class Player {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Player();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Player();

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

	void MovementInput();

	WorldTransform& GetWorldTransform() { return worldTransform_; };

	const Vector3& GetVelocity() const { return velocity_; };

	Vector3 CornerPosition(const Vector3& center, Corner corner);

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

	// void MapCollisionDetection(CollisionMapInfo& info);

	void MapCollisionDetectionUp(CollisionMapInfo& info);

	void MapCollisionDetectionDown(CollisionMapInfo& info);

	void MapCollisionDetectionRight(CollisionMapInfo& info);

	void MapCollisionDetectionLeft(CollisionMapInfo& info);

	// 判定結果を反映して移動させる
	void MoveJudgmentResults(const CollisionMapInfo& info);

	// 壁に接触している場合の処理
	void WallContactProcess(const CollisionMapInfo& info);

	// 接地状態の切り替え処理
	void GroundingStateSwitching(const CollisionMapInfo& info);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// AABBを取得
	AABB GetAABB();

	// 敵との衝突判定
	void OnCollision(const Enemy* enemy);

	// 弾との当たり判定
	void OnCollisionBullet();

	// 棘との衝突判定
	void OnCollision(const Needle* needle);

	// ゴールとの衝突判定
	void OnCollision(const Goal* goal);

	// デスフラグのgetter
	bool IsDead() const { return isDead_; }

	// クリアフラグのgetter
	bool IsClear() const { return isClear_; }

private:
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// カメラ
	ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;

	// モデル
	Model* model_ = nullptr;

	Vector3 velocity_ = {};

	static inline const float kBlank = 0.1f;

	static inline const float kAcceleration = 0.1f;

	static inline const float kLimitRunSpeed = 0.3f;

	static inline const float kAttenuationLanding = 0.1f;

	static inline const float kAdsorptionLanding = 0.2f;

	static inline const float kAttenuationWall = 0.1f;

	// キャラクターの当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;

	// 旋回開始時の角度
	float turnFirstRotationY_ = 0.0f;
	// 旋回タイマー
	float turnTimer_ = 0.0f;
	// 旋回時間<秒>
	static inline const float kTimeTurn = 0.3f;

	// 接地状態フラグ
	bool onGround_ = true;

	// デスフラグ
	bool isDead_ = false;

	// 弾に当たったフラグ
	bool isHit_ = false;

	// クリアフラグ
	bool isClear_ = false;

	// 重力加速度（下方向）
	static inline const float kGravityAcceleration = 0.05f;

	// 最大落下速度（下方向
	static inline const float kLimitFallSpeed = 0.4f;

	// ジャンプ初速（上方向）
	static inline const float kJumpAcceleration = 0.8f;

	static inline const float kAttenuation = 0.3f;

	// 左右
	enum class LRDirection {
		kRight,
		kLeft,
	};

	LRDirection lrDirection_ = LRDirection::kRight;

	// テスト
	static inline const int kEscapeCountMax = 5;
	int escapeCount = 0;
};