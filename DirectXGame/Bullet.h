#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "struct.h"

class MapChipField;
class Player;

class Bullet {
public:

	Bullet();
	~Bullet();

	void Initialize(Model* model, ViewProjection* viewProjection, Vector3& position, Vector3& shotPos);
	void Update();
	void Draw();
	void OnCollision(const Player* player, Vector3 position);

	Vector3 GetWorldPosition() const { return worldTransform_.translation_; };

	// AABBを取得
	AABB GetAABB();

	// 弾の左右の速度制限
	static inline const float kSpeedLeft = -0.5;

	static inline const float kSpeedRight = 0.5f;

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; };

private:
	// マップチップによるフィールド
	MapChipField* mapChipField_ = nullptr;
	//モデル
	Model* model_ = Model::CreateFromOBJ("enemyBullet", true);
	// カメラ
	ViewProjection* viewProjection_ = nullptr;
	// ワールド変換データ
	WorldTransform worldTransform_;
	//速度
	Vector3 velocity_ = {};

	bool isShot_ = false;

	int shotTimer_ = 0;

	static inline const float kWidth = 1.0f;

	static inline const float kHeight = 1.0f;

	static inline const int kIntervalTimer = 120;

	Vector3 shotPos_ = 0;

};
