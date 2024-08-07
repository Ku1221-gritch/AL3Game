#pragma once
#define NOMINMAX
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Rect.h"

//前方宣言
class Player;

class CameraController {
public:

	void Initialize();

	void Update();

	void SetTarget(Player* target) { target_ = target; }

	void Reset();

	const ViewProjection& GetViewProjection() { return viewProjection_; };

	void SetMoveableArea(Rect area) { movableArea_ = area; }

private:
	// トランスフォーム
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;


	Player* target_ = nullptr;

	//追従対象とカメラの座標の差（オフセット）
	Vector3 targetOffset_ = {0, 0, -40.0f};

	//カメラ移動範囲
	Rect movableArea_ = {0, 100, 0, 0};

	//カメラの目標座標
	Vector3 targetCoordinates;

	//速度掛け算
	static inline const float kVelocityBias = 5.0f;

	//座標補間割合
	static inline const float kInterpolationRate = 0.5f;

};