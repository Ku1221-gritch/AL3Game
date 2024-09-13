#define NOMINMAX

#include "TitleScene.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

TitleScene::~TitleScene() {
	delete modelTitle_;
	delete modelPlayer_;
	delete fade_;
}

void TitleScene::Initialize() {
	audio_ = Audio::GetInstance();
	modelTitle_ = Model::CreateFromOBJ("text", true);
	modelPlayer_ = Model::CreateFromOBJ("player");
	modelText_ = Model::CreateFromOBJ("pressthespaceText",true);
	// スカイドーム
	modelSkydome_ = Model::CreateFromOBJ("titleskydome", true);
	Skydome_ = new Skydome();
	Skydome_->Initialize(modelSkydome_);
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();

	const float kPlayerScale = 10.0f;
	worldTransformPlayer_.Initialize();
	worldTransformPlayer_.scale_ = {kPlayerScale, kPlayerScale, kPlayerScale};
	worldTransformPlayer_.rotation_.y = 0.95f * std::numbers::pi_v<float>;
	worldTransformPlayer_.translation_.y = -5.0f;

	const float kTitleTextScale = 10.0f;
	worldTransformTitle_.Initialize();
	worldTransformTitle_.scale_ = {kTitleTextScale, kTitleTextScale, kTitleTextScale};
	worldTransformTitle_.rotation_.y = 0.99f * std::numbers::pi_v<float>;

	worldTransformText_.Initialize();
	worldTransformText_.scale_ = {kTitleTextScale, kTitleTextScale, kTitleTextScale};
	worldTransformText_.rotation_.y = 1.03f * std::numbers::pi_v<float>;
	fade_ = new FadeEffect();
	fade_->Initialize(&viewProjection_, 0.0f, { 0, 0, -40 }, true);
	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("music/MECHANICAL_DEATH.wav");
	// 音楽再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	fade_->Initialize(&viewProjection_, 0.0f, 0.0f, {0, 0, -40}, false,kSquare);
}



void TitleScene::Update() {

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		fade_->canFade_ = true;
		// 音楽停止
		audio_->StopWave(voiceHandle_);
	}
	if (fade_->canFade_) {
		fade_->Update();
		fade_->FadeIn();
	}
  
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTitleMove);

	float angle = counter_ / kTimeTitleMove * 2.0f * std::numbers::pi_v<float>;

	worldTransformTitle_.translation_.y = std::sin(angle) + 10.0f;
	worldTransformText_.translation_.y = std::sin(angle) + 0.0f;

	// スカイドームの更新処理
	Skydome_->Update();

	viewProjection_.TransferMatrix();
	worldTransformTitle_.UpdateMatrix();
	worldTransformPlayer_.UpdateMatrix();
	worldTransformText_.UpdateMatrix();
}

void TitleScene::Draw() {
	DirectXCommon* dxCommon_ = DirectXCommon::GetInstance();
	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	Model::PreDraw(commandList);
	// スカイドームの描画処理
	Skydome_->Draw(&viewProjection_);
	modelTitle_->Draw(worldTransformTitle_, viewProjection_);
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_);
	modelText_->Draw(worldTransformText_, viewProjection_);
	fade_->Draw();
	Model::PostDraw();
}