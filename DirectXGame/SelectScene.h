#pragma once

#include "Audio.h"
#include "CameraController.h"
#include "DeathParticles.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "Function.h"
#include "GameOverText.h"
#include "Goal.h"
#include "Input.h"
#include "MapChipField.h"
#include "Model.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

/// <summary>
/// ゲームシーン
/// </summary>
class SelectScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	SelectScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~SelectScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//ステージ1に進む
	bool ProceedStage1_() const { return proceedStage1_; }
	//ステージ2に進む
	bool ProceedStage2_() const { return proceedStage2_; }
	//ステージ3に進む
	bool ProceedStage3_() const { return proceedStage3_; }
	//タイトルに戻る
	bool IsBackTitle_() const { return backTitle_; }

private: // メンバ変数

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//操作説明のモデル
	Model* modelMoveText_ = nullptr;
	WorldTransform worldTransformMoveText_;
	Model* modelJumpText_ = nullptr;
	WorldTransform worldTransformJumpText_;
	Model* modelBackText_ = nullptr;
	WorldTransform worldTransformBackText_;
	//ステージ入るところ
	Model* modelEntrance1_ = nullptr;
	WorldTransform worldTransformEntrance1_;
	Model* modelEntrance2_ = nullptr;
	WorldTransform worldTransformEntrance2_;
	Model* modelEntrance3_ = nullptr;
	WorldTransform worldTransformEntrance3_;
	Model* modelStage1_ = nullptr;
	WorldTransform worldTransformStage1_;
	Model* modelStage2_ = nullptr;
	WorldTransform worldTransformStage2_;
	Model* modelStage3_ = nullptr;
	WorldTransform worldTransformStage3_;
	Model* modelEnterText_ = nullptr;
	WorldTransform worldTransformEnterText_;
	
	// 画像
	Sprite* sprite_ = nullptr;
	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	// ブロック
	Model* modelBlock_ = nullptr;
	// マップチップフィールド
	MapChipField* mapChipField_;
	// プレイヤー
	Model* modelPlayer_ = nullptr;
	Model* modelMeltPlayer_ = nullptr;
	Player* player_ = nullptr;
	Vector3 playerPosition;
	// ゴール
	Model* modelGoal_ = nullptr;
	Goal* goal_ = nullptr;
	// デスパーティクル
	Model* modelDeathParticle_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	// カメラコントローラー
	CameraController* cameraController_ = nullptr;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	// ハンドル
	uint32_t textureHandle_ = 0;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	// トランスフォーム
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	// ImGuiで値を入力する変数
	float inputFloat3[3] = {0.0f, 0.0f, 0.0f};
	std::vector<std::vector<WorldTransform*>> worldTransformMapChip_;
	// カメラ移動範囲
	Rect movableArea_ = {};
	void GenerateBlocks();
	// 全ての当たり判定を行う
	void CheckAllCollisions();
	// AABB同士の交差判定
	bool IsCollision(const AABB& aabb1, const AABB& aabb2);

	//ステージ1に進む
	bool proceedStage1_ = false;
	//ステージ2に進む
	bool proceedStage2_ = false;
	//ステージ3に進む
	bool proceedStage3_ = false;
	//タイトルに戻る
	bool backTitle_ = false;

	//ウゴクハネル用
	static inline const float kTimeTextMove = 2.0f;
	float counter_ = 0.0f;
	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
