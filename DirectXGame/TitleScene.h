#pragma once

#include "FadeEffect.h"
#include "Model.h"
#include "Skydome.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class FadeEffect;

class TitleScene {
public:
	~TitleScene();

	void Initialize();

	void Update();

	void Draw();

	FadeEffect* fade_ = nullptr;

private:
	static inline const float kTimeTitleMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;

	Model* modelTitle_ = nullptr;
	Model* modelPlayer_ = nullptr;
	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;

	float counter_ = 0.0f;

};