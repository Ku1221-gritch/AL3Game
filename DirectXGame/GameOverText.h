#pragma once

#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class GameOverText {
public:
	~GameOverText();

	void Initialize();

	void Update();

	void Draw();

private:
	static inline const float kTimeTextMove = 2.0f;

	ViewProjection viewProjection_;
	WorldTransform worldTransform_;
	Model* model_ = nullptr;

	float counter_ = 0.0f;
};