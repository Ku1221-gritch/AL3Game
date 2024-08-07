#define NOMINMAX
#include "DeathParticles.h"
#include "Function.h"
#include <cassert>

void DeathParticles::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	// NULLポインタチェック
	assert(model);

	model_ = model;

	for (auto& worldTransform : worldTransforms) {
		worldTransform.Initialize();
		worldTransform.translation_ = position;
	}
	viewProjection_ = viewProjection;
	
	objectColor_.Initialize();
	color_ = {1, 1, 1, 1};
}

void DeathParticles::Update() {
	// 終了なら何もしない
	if (isFinished_) {
		return;
	}

	//カウンターを1フレーム分の秒数
	counter += 1.0f / 60.0f;

	//存続時間の上限に達したら
	if (counter >= kDuration) {
		counter = kDuration;
		//終了扱いにする
		isFinished_ = true;
	}

	for (uint32_t i = 0; i < worldTransforms.size(); ++i) {
		//基本となる速度ベクトル
		Vector3 velocity = {kSpeed, 0, 0};
		//回転角を計算する
		float angle = kAngleUnit * i;
		//Z軸まわり回転行列
		Matrix4x4 matrixRotation = MakeRotateZMatrix(angle);
		//基本ベクトルを回転させて速度ベクトルを得る
		velocity = Transform(velocity, matrixRotation);
		//移動処理
		worldTransforms[i].translation_ += velocity;
	}

	for (auto& worldTransform : worldTransforms) {
		worldTransform.UpdateMatrix();
	}

	color_.w = std::max(0.0f, 1.0f - counter / kDuration);
	//色変更オブジェクトに色の数値を設定する
	objectColor_.SetColor(color_);
	//色変更オブジェクトをVRAMに転送
	objectColor_.TransferMatrix();
}

void DeathParticles::Draw() {
	//終了なら何もしない
	if (isFinished_) {
		return;
	}

	for (auto& worldTransform : worldTransforms) {
		model_->Draw(worldTransform, *viewProjection_,&objectColor_); 
	}
}