#define NOMINMAX

#include "DirectXCommon.h"
#include "Input.h"
#include "ClearScene.h"
#include <cmath>
#include <numbers>

ClearScene::~ClearScene() {
	delete modelClear_;
	delete modelPlayer_;
}

void ClearScene::Initialize() {
	modelClear_ = Model::CreateFromOBJ("gameclear", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	// スカイドーム
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.y = -9.0f;

	const float kTextTitle = 10.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTextTitle, kTextTitle, kTextTitle};
	worldTransformTitle_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
}

void ClearScene::Update() {
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		finished_ = true;
	}
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTextMove);

	float angle = counter_ / kTimeTextMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;

	// スカイドームの更新処理
	skydome_->Update();

	viewProjection_.TransferMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
}

void ClearScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	// スカイドームの描画処理
	skydome_->Draw(&viewProjection_);
	modelClear_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);

	Model::PostDraw();
}