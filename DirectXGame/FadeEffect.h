#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

enum FadeMode { kSquare, kCircle };

class FadeEffect {

public:
	FadeEffect();

	~FadeEffect();

	void Initialize(ViewProjection* viewProjection, float squareAlpha, float circleAlpha, Vector3 position, bool onOff, FadeMode mode);

	void Update();

	void Draw();

	void FadeIn();

	void FadeOut();

	void FadeUp();

	void FadeInCircle(Vector3 position);

	void FadeOutCircle(Vector3 position);

	bool IsFinished() const { return fadeStart_; };

	bool IsFadeDone() const { return isFadeDone_; };

	// フェードの許可
	bool canFade_ = false;

	FadeMode fadeMode_ = FadeMode::kSquare;

	float GetAlpha() const { return squareAlpha_; };

	void SetCircleScale() {
		worldTransformCircle_.scale_.x = 400;
		worldTransformCircle_.scale_.y = 400;
	};

private:
	ViewProjection* viewProjection_;
	WorldTransform worldTransformSquare_;
	WorldTransform worldTransformCircle_;
	Model* modelSquare_ = nullptr;
	Model* modelCircle_ = nullptr;

	// アルファ値
	float squareAlpha_;
	float circleAlpha_;
	// フェード速度
	float alphaValue_;
	// 色
	ObjectColor* squareColor_ = nullptr;
	ObjectColor* circleColor_ = nullptr;

	// フェードが完了しているかどうか
	bool isFadeDone_ = false;
	// フェードの開始
	bool fadeStart_ = false;

	// フェードスケール
	Vector3 fadeScale_;
};
