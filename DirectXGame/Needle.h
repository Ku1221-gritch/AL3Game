#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "struct.h"

class MapChipField;
class Player;

class Needle {
public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Needle();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Needle();

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
	// 最初の角度[度]
	static inline const float kWalkMotionAngleStart = 0.1f;
	// 最後の角度[度]
	static inline const float kWalkMotionAngleEnd = 5.0f;
	// アニメーションの周期となる時間[秒]
	static inline const float kWalkMotionTime = 0.5f;
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

private:
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	// カメラ
	ViewProjection* viewProjection_ = nullptr;
	// モデル
	Model* model_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	// 棘の当たり判定サイズ
	static inline const float kWidth = 1.8f;
	static inline const float kHeight = 1.8f;
};
