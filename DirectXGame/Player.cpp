#define NOMINMAX
#include <algorithm>
#include "TextureManager.h"
#include "GameScene.h"
#include "Player.h"
#include <cassert>
#include <numbers>
#include <iostream>
#include"Rect.h"
#include "MapChipField.h"

Player::Player() {}

Player::~Player() {}

void Player::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& position) {
	//引数の内容をメンバ変数に記録
	model_ = model;
	viewProjection_ = viewProjection;
	//NULLポインタチェック
	assert(model);

	//ワールド変換の初期化
	worldTransform_.Initialize();
	worldTransform_.translation_ = position;
	worldTransform_.rotation_.y = std::numbers::pi_v<float> / 2.0f;
}

void Player::Update() { 
	//移動処理
	MovementInput();

	//着地フラグ
	bool landing = false;

	//地面との当たり判定
	// 下降中？
	if (velocity_.y < 0) {
		//Y座標が地面以下になったら着地
		if (worldTransform_.translation_.y <= 2.0f) {
			landing = true;
		}
	}

	//接地判定
	if (onGround_) {
		//ジャンプ開始
		if (velocity_.y > 0.0f) {
			//空中状態に移行
			onGround_ = false;
		}
	} else {
		//着地
		if (landing) {
			//めり込み排斥
			worldTransform_.translation_.y = 2.0f;
			//摩擦で横方向速度が減衰する
			velocity_.x *= (1.0f - kAttenuation);
			//下方向速度をリセット
			velocity_.y = 0.0f;
			//接地状態に移行
			onGround_ = true;
		}
	}

	// 衝突情報を初期化
	CollisionMapInfo collisionMapinfo;

	// 移動量に速度の値をコピー
	collisionMapinfo.movement_ = velocity_;

	// マップ衝突チェック
	// MapCollisionDetection(collisionMapinfo);
	MapCollisionDetectionUp(collisionMapinfo);
	MapCollisionDetectionDown(collisionMapinfo);
	MapCollisionDetectionRight(collisionMapinfo);
	MapCollisionDetectionLeft(collisionMapinfo);

	// 接地状態の切り替え処理
	GroundingStateSwitching(collisionMapinfo);

	// 壁に接触している場合の処理
	WallContactProcess(collisionMapinfo);

	//
	MoveJudgmentResults(collisionMapinfo);


	//旋回制御
	{
		if (turnTimer_ > 0.0f) {
			turnTimer_ -= 1.0f / 60.0f;
			// 左右の自キャラ角度テーブル
			float destinationRotationYTable[] = {std::numbers::pi_v<float> / 2.0f, std::numbers::pi_v<float> * 3.0f / 2.0f};
			
			// 状態に応じた角度を取得する
			float destinationRotationY = destinationRotationYTable[static_cast<uint32_t>(lrDirection_)];

			// 自キャラの角度を設定する
			worldTransform_.rotation_.y = std::lerp(turnFirstRotationY_, destinationRotationY, 1 - turnTimer_ / kTimeTurn);
	 	}
	}

	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Player::Draw() {
	model_->Draw(worldTransform_, *viewProjection_); 
}

//移動入力
void Player::MovementInput() {

	// 左右移動操作
	if (Input::GetInstance()->PushKey(DIK_RIGHT) || Input::GetInstance()->PushKey(DIK_LEFT)) {

		// 左右加速
		Vector3 acceleration = {};
		if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
			// 左移動中の右入力
			if (velocity_.x < 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			if (lrDirection_ != LRDirection::kRight) {
				lrDirection_ = LRDirection::kRight;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
			acceleration.x += kAttenuation;
		} else if (Input::GetInstance()->PushKey(DIK_LEFT)) {

			// 右移動中の左入力
			if (velocity_.x > 0.0f) {
				// 速度と逆方向に入力中は急ブレーキ
				velocity_.x *= (1.0f - kAttenuation);
			}
			if (lrDirection_ != LRDirection::kLeft) {
				lrDirection_ = LRDirection::kLeft;
				turnFirstRotationY_ = worldTransform_.rotation_.y;
				turnTimer_ = kTimeTurn;
			}
			acceleration.x -= kAttenuation;
		}

		// 加速／減速
		velocity_ += acceleration;

		// 最大速度制限
		velocity_.x = std::clamp(velocity_.x, -kLimitRunSpeed, kLimitRunSpeed);
	} else {
		// 非入力時は移動減衰をかける
		velocity_.x *= (1.0f - kAttenuation);
	}
	// 接地状態
	if (onGround_) {
		if (Input::GetInstance()->PushKey(DIK_UP)) {
			// ジャンプ初速
			velocity_ += Vector3(0, kJumpAcceleration, 0);
		}

		// 空中
	}else{
		// 落下速度
		velocity_ += Vector3(0, -kGravityAcceleration, 0);
		// 落下速度制限
		velocity_.y = (std::max)(velocity_.y, -kLimitFallSpeed);
	}
}


void Player::MapCollisionDetectionUp(CollisionMapInfo& info) 
{
	// 上昇あり？
	if (info.movement_.y <= 0) {
		return;
	}
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;
	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement_, static_cast<Corner>(i));
	}

	MapChipType mapChipType;
	IndexSet indexSet;
	//真上の当たり判定を行う
	bool hit = false;
	//左上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop] + Vector3(0.2f, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
		
	}
	//右上点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop] + Vector3(-0.2f, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	//ブロックにヒット？
	if (hit) {
		//めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
		//めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = worldTransform_.translation_.y - rect.top;
		
		info.movement_.y = std::max(0.0f, moveY);
		//天井に当たったことを記録する
		info.onCeiling_ = true;

	}

	if (info.onCeiling_) 
	{
		velocity_.y = 0.0f;
	}
}

void Player::MapCollisionDetectionDown(CollisionMapInfo& info) 
{
	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement_, static_cast<Corner>(i));
	}

	// 下降あり？
	if (info.movement_.y >= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左下点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0.2f,0,0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(-0.2f, 0, 0));
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		float moveY = worldTransform_.translation_.y + rect.top;

		info.movement_.y = std::min(0.0f, moveY);
		// 天井に当たったことを記録する
		info.onLanding_ = true;
	}
}

void Player::MapCollisionDetectionRight(CollisionMapInfo& info) {

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement_, static_cast<Corner>(i));
	}

	//右移動あり？
	if (info.movement_.x <= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 右上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 右下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement_.x = std::max(0.0f, rect.left - worldTransform_.translation_.x - kWidth / 2 - kBlank);
		// 壁に当たったことを記録する
		info.onWallcontact = true;
	}

}

void Player::MapCollisionDetectionLeft(CollisionMapInfo& info) {

	// 移動後の4つの角の座標
	std::array<Vector3, kNumCorner> positionsNew;

	for (uint32_t i = 0; i < positionsNew.size(); ++i) {
		positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement_, static_cast<Corner>(i));
	}

	// 左移動あり？
	if (info.movement_.x >= 0) {
		return;
	}

	MapChipType mapChipType;
	// 真下の当たり判定を行う
	bool hit = false;
	// 左上点の判定
	IndexSet indexSet;
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftTop]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}
	// 左下点の判定
	indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom]);
	mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
	if (mapChipType == MapChipType::kBlock) {
		hit = true;
	}

	// ブロックにヒット？
	if (hit) {
		// めり込みを排除する方向に移動量を設定する
		indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightTop]);
		// めり込み先ブロックの範囲矩形
		Rect rect = mapChipField_->GetRectByIndex(indexSet.xIndex, indexSet.yIndex);
		info.movement_.x = std::min(0.0f, rect.left - worldTransform_.translation_.x + kWidth / 2 + kBlank);
		// 壁に当たったことを記録する
		info.onWallcontact = true;
	}

}

// 判定結果を反映して移動させる
void Player::MoveJudgmentResults(const CollisionMapInfo& info) {
	// 移動
	worldTransform_.translation_ += info.movement_;
}

// 壁に接触している場合の処理
void Player::WallContactProcess(const CollisionMapInfo& info) {
	//壁接触による減速
	if (info.onWallcontact) {
		velocity_.x *= (1.0f - kAttenuationWall);
	}
}

// 接地状態の切り替え処理
void Player::GroundingStateSwitching(const CollisionMapInfo& info) {

	//自キャラが接地状態？
	if (onGround_) {
		// ジャンプ開始
		if (velocity_.y > 0.0f) {
			onGround_ = false;
		} else {
			// 移動後の4つの角の座標
			std::array<Vector3, kNumCorner> positionsNew;

			for (uint32_t i = 0; i < positionsNew.size(); ++i) {
				positionsNew[i] = CornerPosition(worldTransform_.translation_ + info.movement_, static_cast<Corner>(i));
			}

			MapChipType mapChipType;
			// 真下の当たり判定を行う
			bool downHit = false;
			// 左下点の判定
			IndexSet indexSet;
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kLeftBottom] + Vector3(0, -kAdsorptionLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				downHit = true;
			}
			// 右下点の判定
			indexSet = mapChipField_->GetMapChipIndexSetByPosition(positionsNew[kRightBottom] + Vector3(0, -kAdsorptionLanding, 0));
			mapChipType = mapChipField_->GetMapChipTypeByIndex(indexSet.xIndex, indexSet.yIndex);
			if (mapChipType == MapChipType::kBlock) {
				downHit = true;
			}

			// 落下開始
			if (!downHit) {
				// 空中状態に切り替える
				onGround_ = false;
			}
		}
	} else {
		
		// 着地フラグ
		if (info.onLanding_) {
			// 着地状態に切り替える（落下を止める）
			onGround_ = true;
			// 着地時にX速度を減衰
			velocity_.x *= (1.0f - kAttenuationLanding);
			// Y速度をゼロにする
			velocity_.y = 0.0f;
		}

	}
	
}

Vector3 Player::GetWorldPosition() {
	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_.translation_.x;
	worldPos.y = worldTransform_.translation_.y;
	worldPos.z = worldTransform_.translation_.z;

	return worldPos;
}

AABB Player::GetAABB() {
	
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

//プレイヤーと敵の当たった場合の処理
void Player::OnCollision(const Enemy* enemy) { 
	(void)enemy; 
	//デスフラグを立てる
	//isDead_ = true;
}

Vector3 Player::CornerPosition(const Vector3& center, Corner corner) {
	Vector3 offsetTable[kNumCorner] = {
	    {+kWidth / 2.0f, -kHeight / 2.0f, 0},
        {-kWidth / 2.0f, -kHeight / 2.0f, 0},
        {+kWidth / 2.0f, +kHeight / 2.0f, 0},
        {-kWidth / 2.0f, +kHeight / 2.0f, 0}
    };
	
	return center + offsetTable[static_cast<uint32_t>(corner)];
}
