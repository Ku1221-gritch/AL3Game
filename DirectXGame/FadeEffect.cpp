#include "FadeEffect.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

FadeEffect::FadeEffect() {}

FadeEffect::~FadeEffect() {}

void FadeEffect::Initialize(ViewProjection* viewProjection, float alpha, Vector3 position, bool onOff) {

	// ビュープロジェクションの初期化
	viewProjection_ = viewProjection;

	// フェードイン
	worldTransformFade_.Initialize();
	worldTransformFade_.translation_ = position;
	worldTransformFade_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	modelFade_ = Model::CreateFromOBJ("blackBoard", true);
	canFade_ = onOff;
	alpha_ = alpha;
	alphaValue_ = 0.05f;
	color_ = new ObjectColor();
	color_->Initialize();
	color_->SetColor({1, 1, 1, alpha_});
	color_->TransferMatrix();
}

void FadeEffect::Update() {
	if (canFade_ && Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		fadeStart_ = true;
	}
	if (fadeStart_) {
		FadeIn();
	}
	if (isFadeDone_) {
		FadeOut();
	}
	if (alpha_ > 1.2f) {
		alpha_ = 1.2f;
		isFadeDone_ = true;
	}
	if (alpha_ < 0.0f) {
		isFadeDone_ = false;
		fadeStart_ = false;
	}

	worldTransformFade_.UpdateMatrix();
}

void FadeEffect::Draw() { modelFade_->Draw(worldTransformFade_, *viewProjection_, color_); }

void FadeEffect::FadeIn() {
	alpha_ += alphaValue_;
	color_->SetColor({1, 1, 1, alpha_});
	color_->TransferMatrix();
	worldTransformFade_.UpdateMatrix();
}

void FadeEffect::FadeOut() {
	alpha_ -= alphaValue_;
	color_->SetColor({1, 1, 1, alpha_});
	color_->TransferMatrix();
	worldTransformFade_.UpdateMatrix();
}
