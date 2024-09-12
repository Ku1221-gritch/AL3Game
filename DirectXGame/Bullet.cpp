#include "Bullet.h"
#include "Player.h"

Bullet::Bullet() {}

Bullet::~Bullet() {}

void Bullet::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	model_ = model;
	viewProjection_ = viewProjection;
	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	velocity_ = {kSpeedLeft, 0};
}

void Bullet::Update(Vector3 position) {
	BulletShot(position);

	worldTransform_.translation_.x += velocity_.x;

	// 回転アニメーション
	float radian = float(std::sin(shotTimer_));
	worldTransform_.rotation_.x = radian;

	worldTransform_.UpdateMatrix();
}

void Bullet::Draw() { model_->Draw(worldTransform_, *viewProjection_); }

void Bullet::OnCollision(const Player* player, Vector3 position) {
	(void)player;
	worldTransform_.translation_ = position;
}

Vector3 Bullet::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

void Bullet::BulletShot(Vector3 position) {
	shotTimer_++;

	if (shotTimer_ >= kIntervalTimer) {
		shotTimer_ = 0;
		isShot_ = true;
	}
	if (isShot_) {
		worldTransform_.translation_ = position;
		isShot_ = false;
	}
}

AABB Bullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}
