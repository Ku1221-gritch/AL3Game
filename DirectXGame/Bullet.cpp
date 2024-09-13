#include "Bullet.h"
#include "Player.h"
#include "GameScene.h"

Bullet::Bullet() {}

Bullet::~Bullet() {}

void Bullet::Initialize(Model* model, ViewProjection* viewProjection,  Vector3& position,Vector3& shotPos) {
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	// 速度を設定する
	velocity_ = {kSpeedLeft, 0};
	shotPos_ = shotPos;
}

void Bullet::Update() {

	// 回転アニメーション
	float radian = float(std::sin(shotTimer_));
	worldTransform_.rotation_.x = radian;
	shotTimer_++;

	if (shotTimer_ >= kIntervalTimer) {
		shotTimer_ = 0;
		isShot_ = true;
	}
	if (isShot_) {
		worldTransform_.translation_ = shotPos_;
		isShot_ = false;
	}
	worldTransform_.UpdateMatrix();
}

void Bullet::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Bullet::OnCollision(const Player* player, Vector3 position) {
	(void)player;
	worldTransform_.translation_ = position;
}


AABB Bullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
