#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "Sprite.h"
#include "Player.h"
#include "Enemy.h"
#include"Skydome.h"
#include"Goal.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include "DebugCamera.h"
#include<vector>
#include "Function.h"
#include "MapChipField.h"
#include"DeathParticles.h"
#include"GameOverText.h"
#include "CameraController.h"

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

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

	//デスフラグのgetter
	bool IsDeathFinished() const { return deathFinished_; };
	//クリアフラグのgetter
	bool IsClear() const { return clearFinished_; };

private: // メンバ変数
	//ゲームのフェーズ（型）
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

	//ゲームの現在フェーズ（変数）
	Phase phase_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	//画像
	Sprite* sprite_ = nullptr;
	//スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* skydome_ = nullptr;
	//ブロック
	Model* modelBlock_ = nullptr;
	//マップチップフィールド
	MapChipField* mapChipField_;
	// プレイヤー
	Model* modelPlayer_ = nullptr;
	Player* player_ = nullptr;
	//敵
	Model* modelEnemy_ = nullptr;
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	//ゴール
	Model* modelGoal_ = nullptr;
	Goal* goal_ = nullptr;
	//デスパーティクル
	Model* modelDeathParticle_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	//ゲームオーバーテキスト
	Model* modelGameOverText_ = nullptr;
	GameOverText* gameOverText_ = nullptr;
	//カメラコントローラー
	CameraController* cameraController_ = nullptr;
	//デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	//ハンドル
	uint32_t textureHandle_ = 0;
	uint32_t soundDataHandle_ = 0;
	uint32_t voiceHandle_ = 0;
	//トランスフォーム
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	//ImGuiで値を入力する変数
	float inputFloat3[3] = {0.0f, 0.0f, 0.0f};
	std::vector<std::vector<WorldTransform*>> worldTransformBlocks_;
	// カメラ移動範囲
	Rect movableArea_ = {};
	void GenerateBlocks();
	// 全ての当たり判定を行う
	void CheckAllCollisions();
	// AABB同士の交差判定
	bool IsCollision(const AABB& aabb1, const AABB& aabb2);
	//フェーズの切り替え
	void ChangePhase();
	//死亡による終了フラグ
	bool deathFinished_ = false;
	//クリアによる終了フラグ
	bool clearFinished_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
