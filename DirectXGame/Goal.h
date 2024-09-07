#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"
#include "struct.h" 
#include <numbers> 

class MapChipField;
class Player;

class Goal {
public:
	Goal();
	~Goal();
	void Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position);
	void Update();
	void Draw();

	static inline const float kWidth = 2.0f;
	static inline const float kHeight = 2.0f;

	void SetMapChipField(MapChipField* mapChipField) { mapChipField_ = mapChipField; }

	Vector3 GetWorldPosition();
	AABB GetAABB();
	void OnCollision(const Player* player);

	bool isGoal() const { return isClear_; }

private:
	Model* model_ = nullptr;
	Vector3 velocity_ = {};
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_;
	bool isClear_ = false;

	MapChipField* mapChipField_ = nullptr;
};
