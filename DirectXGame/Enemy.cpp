#include "Enemy.h"
#include "GameScene.h"
#include <cassert>
#include <numbers>
#include <cmath>

Enemy::Enemy() {}

Enemy::~Enemy() {}

void Enemy::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// 引数の内容をメンバ変数に記録
	model_ = model;
	viewProjection_ = viewProjection;
	// NULLポインタチェック
	assert(model);

	// ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;

	//速度を設定する
	velocity_ = {-kWalkSpeed, 0, 0};

	walkTimer_ = 0.0f;
}

void Enemy::Update() {

	//移動
	worldTransform_.translation_ += velocity_;

	// タイマーを加算
	walkTimer_ += 6.0f / 60.0f;

	//回転アニメーション
	float param = std::sin(walkTimer_);
	float radian = kWalkMotionAngleStart + kWalkMotionAngleEnd * (param + 1.0f) / 2.0f;
	worldTransform_.rotation_.z = std::sin(radian);

	//行列計算
	worldTransform_.UpdateMatrix();
}

void Enemy::Draw() { 
	model_->Draw(worldTransform_, *viewProjection_); 
}

Vector3 Enemy::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Enemy::GetAABB() {

	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Enemy::OnCollision(const Player* player) {
	(void)player;
}
