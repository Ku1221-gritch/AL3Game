#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"

// マップとの当たり判定情報
struct CollisionMapInfo {
	// 天井衝突フラグ
	bool onCeiling_ = false;
	// 着地フラグ
	bool onLanding_ = false;
	// 壁接触フラグ
	bool onWallcontact = false;
	Vector3 movement_;
};

// 角
enum Corner {
	kRightBottom, // 右下
	kLeftBottom,  // 左下
	kRightTop,    // 右上
	kLeftTop,     // 左上

	kNumCorner // 要素数
};

//AABB当たり判定
struct AABB {
	Vector3 max;
	Vector3 min;
};