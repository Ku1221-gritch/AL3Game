#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "struct.h"

class Player;

class Bullet {
public:

	Bullet();
	~Bullet();

	void Initialize(Model* model,ViewProjection* viewProjection, const Vector3& position);
	void Update(Vector3 position);
	void OnCollision(const Player* player);

	Vector3 GetWorldPosition();
	void BulletShot(Vector3 position);

	// AABBを取得
	AABB GetAABB();

private:
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

	static inline const int kIntervalTimer = 240;

};
