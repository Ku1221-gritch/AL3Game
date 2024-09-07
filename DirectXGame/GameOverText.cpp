#include "GameOverText.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

GameOverText::~GameOverText() {
	delete model_; 
}

void GameOverText::Initialize() {
	// ゲームオーバーテキストの生成
	model_ = Model::CreateFromOBJ("gameover", true);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
	const float kGameOverTextScale = 10.0f;
	worldTransform_.Initialize();
	worldTransform_.scale_ = {kGameOverTextScale, kGameOverTextScale, kGameOverTextScale};
	worldTransform_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransform_.translation_.y = -9.0f;
}

void GameOverText::Update() {
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTextMove);

	float angle = counter_ / kTimeTextMove * 2.0f * std::numbers::pi_v<float>;

	worldTransform_.translation_.y = std::sin(angle) + 10.0f;

	viewProjection_.TransferMatrix();
	worldTransform_.UpdateMatrix();
}

void GameOverText::Draw() {
	model_->Draw(worldTransform_, viewProjection_);
}
