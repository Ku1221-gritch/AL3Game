#pragma once

#include "Audio.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"Skydome.h"

class ClearScene {
public:
	~ClearScene();

	void Initialize();

	void Update();

	void Draw();

	bool IsFinished() const { return finished_; }

private:
	static inline const float kTimeTextMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransformTitle_;
	WorldTransform worldTransformPlayer_;
	WorldTransform worldTransformText_;

	Model* modelClear_ = nullptr;
	Model* modelText_ = nullptr;
	Model* modelPlayer_ = nullptr;
	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	
	Audio* audio_ = nullptr;
	// 音楽データ
	uint32_t soundDataHandle_ = 0;
	// 音声再生ハンドル
	uint32_t voiceHandle_ = 0;

	float counter_ = 0.0f;
	bool finished_ = false;
};