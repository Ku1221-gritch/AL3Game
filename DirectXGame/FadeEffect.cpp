#include "FadeEffect.h"
#include "DirectXCommon.h"
#include "Input.h"
#include <cmath>
#include <numbers>

FadeEffect::FadeEffect() {}

FadeEffect::~FadeEffect() {}

void FadeEffect::Initialize(ViewProjection* viewProjection, float squareAlpha, float circleAlpha, Vector3 position, bool onOff, FadeMode mode) {

	// ビュープロジェクションの初期化
	viewProjection_ = viewProjection;

	// フェードモード
	fadeMode_ = mode;
	// フェードSquare
	worldTransformSquare_.Initialize();
	worldTransformSquare_.translation_ = position;
	worldTransformSquare_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
	modelSquare_ = Model::CreateFromOBJ("blackBoard", true);
	canFade_ = onOff;
	squareAlpha_ = squareAlpha;
	alphaValue_ = 0.05f;
	fadeScale_ = {5, 5, 5};
	squareColor_ = new ObjectColor();
	squareColor_->Initialize();
	squareColor_->SetColor({1, 1, 1, squareAlpha_});
	squareColor_->TransferMatrix();
	// フェードCircle
	circleAlpha_ = circleAlpha;
	worldTransformCircle_.Initialize();
	worldTransformCircle_.translation_ = position;
	modelCircle_ = Model::CreateFromOBJ("blackBoard", true);
	circleColor_ = new ObjectColor();
	circleColor_->Initialize();
	circleColor_->SetColor({1, 1, 1, circleAlpha_});
	circleColor_->TransferMatrix();
}

void FadeEffect::Update() {

	if (fadeMode_ == FadeMode::kSquare) {

		if (squareAlpha_ > 1.2f) {
			squareAlpha_ = 1.2f;
			isFadeDone_ = true;
		}
		if (squareAlpha_ < 0.0f) {
			fadeStart_ = false;
		}
	}
	if (fadeMode_ == FadeMode::kCircle) {
		if (worldTransformCircle_.scale_.x > 400.0f) {
			worldTransformCircle_.scale_.x = 400.0f;
			worldTransformCircle_.scale_.y = 400.0f;
			isFadeDone_ = true;
		}
		if (worldTransformCircle_.scale_.x < 0) {
			fadeScale_.x = 0;
			fadeScale_.y = 0;
			circleAlpha_ = 0.0f;
			circleColor_->SetColor({1, 1, 1, circleAlpha_});
			circleColor_->TransferMatrix();			
			isFadeDone_ = false;
			fadeStart_ = false;
		}
	}

	worldTransformSquare_.UpdateMatrix();
}

void FadeEffect::Draw() {
	if (fadeMode_ == kSquare) {
		modelSquare_->Draw(worldTransformSquare_, *viewProjection_, squareColor_);
	} else if (fadeMode_ == kCircle) {
		modelCircle_->Draw(worldTransformCircle_, *viewProjection_, circleColor_);
	}
}

void FadeEffect::FadeIn() {
	squareAlpha_ += alphaValue_;
	squareColor_->SetColor({1, 1, 1, squareAlpha_});
	squareColor_->TransferMatrix();
	worldTransformSquare_.UpdateMatrix();
}

void FadeEffect::FadeOut() {
	isFadeDone_ = false;
	squareAlpha_ -= alphaValue_;
	squareColor_->SetColor({1, 1, 1, squareAlpha_});
	squareColor_->TransferMatrix();
	worldTransformSquare_.UpdateMatrix();
}

void FadeEffect::FadeUp() {
	worldTransformSquare_.translation_.y -= 0.5f;
	worldTransformSquare_.UpdateMatrix();
}

void FadeEffect::FadeInCircle(Vector3 position) {
	worldTransformCircle_.translation_ = position;
	fadeMode_ = kCircle;
	circleAlpha_ = 1.0f;
	circleColor_->SetColor({1, 1, 1, circleAlpha_});
	circleColor_->TransferMatrix();
	worldTransformCircle_.scale_.x += fadeScale_.x;
	worldTransformCircle_.scale_.y += fadeScale_.y;
	worldTransformCircle_.UpdateMatrix();
}

void FadeEffect::FadeOutCircle(Vector3 position) {
	worldTransformCircle_.translation_ = position;
	fadeMode_ = kCircle;
	circleAlpha_ = 1.0f;
	circleColor_->SetColor({1, 1, 1, circleAlpha_});
	circleColor_->TransferMatrix();
	worldTransformCircle_.scale_.x -= fadeScale_.x;
	worldTransformCircle_.scale_.y -= fadeScale_.y;
	worldTransformCircle_.UpdateMatrix();
}
