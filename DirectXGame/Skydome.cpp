#include "Skydome.h"
#include "TextureManager.h"
#include "GameScene.h"
#include"ViewProjection.h"
#include <cassert>

Skydome::Skydome() {

}

Skydome::~Skydome() {

}

void Skydome::Initialize(Model* model ) {
	// 引数の内容をメンバ変数に記録
	model_ = model;

	// NULLポインタチェック
	assert(model);

	worldTransform_.Initialize();
	worldTransform_.translation_ = {0, 0, 0};
	worldTransform_.scale_ = {300,300,300};
}

void Skydome::Update() { 
	worldTransform_.UpdateMatrix(); 
}

void Skydome::Draw(ViewProjection* viewProjection) { 

	model_->Draw(worldTransform_, *viewProjection);
}
