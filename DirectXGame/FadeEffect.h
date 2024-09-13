#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class FadeEffect {

public:
	FadeEffect();

	~FadeEffect();

	void Initialize(ViewProjection* viewProjection, float alpha, Vector3 position, bool onOff);

	void Update();

	void Draw();

	void FadeIn();

	void FadeOut();

	bool IsFinished() const { return fadeStart_; };

	bool IsFadeDone() const { return isFadeDone_; };

private:
	ViewProjection* viewProjection_;
	WorldTransform worldTransformFade_;
	Model* modelFade_ = nullptr;
	// アルファ値
	float alpha_;
	// フェード速度
	float alphaValue_;
	// 色
	ObjectColor* color_ = nullptr;

	// フェードが完了しているかどうか
	bool isFadeDone_ = false;
	//フェードの開始
	bool fadeStart_ = false;
	//フェードの許可
	bool canFade_ = false;

};
