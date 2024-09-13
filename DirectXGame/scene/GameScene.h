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
#include "Needle.h"
#include "Player.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include <vector>

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

	// デスフラグのgetter
	bool IsDeathFinished() const { return deathFinished_; };
	// クリアフラグのgetter
	bool IsClear() const { return clearFinished_; };
	// 戻るフラグのgetter
	bool IsBackSelect() const { return backSelect_; };

private: // メンバ変数
	// ゲームのフェーズ（型）
	enum class Phase {
		kPlay,  // ゲームプレイ
		kDeath, // デス演出
	};

	// ゲームの現在フェーズ（変数）
	Phase phase_;

	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	// 画像
	Sprite* sprite_ = nullptr;
	// スカイドーム
	Model* modelSkydome_ = nullptr;
	Skydome* Skydome_ = nullptr;
	// ブロック
	Model* modelBlock_ = nullptr;
	// 敵
	Model* modelEnemy_ = nullptr;
	Enemy* enemy_ = nullptr;
	std::list<Enemy*> enemies_;
	static inline const int kEnemyMax = 20;
#pragma region 敵一体ずつの配置場所
	intVector2 enemyPos[kEnemyMax] = {
	    {15, 7},
        {38, 14},
        {36, 16},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0},
        {0, 0}
    };
#pragma endregion
	//棘
	Model* modelNeedle_ = nullptr;
	Needle* needle_ = nullptr;
	std::list<Needle*> needles_;
	static inline const int kNeedlesMax = 100;
#pragma region 棘一つずつの配置場所
	intVector2 needlePos[kNeedlesMax] = 
	{
	    {20,12},
        {19, 12},
		{27, 12},
        {26, 12},
        {1,  16},
        {2,  16},
        {3,  16},
        {4,  16},
        {5,  16},
        {6,  16},
        {7,  16},
        {8,  16},
        {9,  16},
        {10, 16},
        {11, 16},
        {12, 16},
        {13, 16},
        {14, 16},
        {15, 16},
        {16, 16},
        {17, 16},
        {18, 16},
        {19, 16},
        {20, 16},
        {21, 16},
        {22, 16},
        {23, 16},
        {24, 16},
        {25, 16},
        {26, 16},
        {27, 16},
        {28, 16},
        {29, 16},
        {30, 16},
        {31, 16},
        {2, 44},
        {3, 44},
        {4, 44},
        {5, 44},
        {6, 44},
        {7, 44},
        {8, 44},
        {9, 44},
        {10, 44},
        {11, 44},
        {12, 44},
        {13, 44},
        {14, 44},
        {15, 44},
        {16, 44},
        {17, 44},
        {18, 44},
        {19, 44},
        {20, 44},
        {21, 44},
        {22, 44},
        {23, 44},
        {24, 44},
        {25, 44},
        {26, 44},
        {27, 44},
        {28, 44},
        {29, 44},
        {30, 44},
        {31, 44},
        {32, 44},
        {33, 44},
        {34, 44},
        {35, 44},
        {36, 44},
        {37, 44},
        {38, 44},
        {39, 44},
        {40, 44},
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
        {0,  0 },
	};
#pragma endregion
	// マップチップフィールド
	MapChipField* mapChipField_;
	//F
	Model* modelF_ = nullptr;
	WorldTransform worldTransformF_;
	static inline const float kTimeTextMove = 0.1f;
	float counter_ = 0.0f;
	// プレイヤー
	Model* modelPlayer_ = nullptr;
	Model* modelMeltPlayer_ = nullptr;
	Player* player_ = nullptr;
	Vector3 playerPosition = 0;
	//ゴール
	Model* modelGoal_ = nullptr;
	Goal* goal_ = nullptr;
	// デスパーティクル
	Model* modelDeathParticle_ = nullptr;
	DeathParticles* deathParticles_ = nullptr;
	// ゲームオーバーテキスト
	Model* modelGameOverText_ = nullptr;
	GameOverText* gameOverText_ = nullptr;
	// カメラコントローラー
	CameraController* cameraController_ = nullptr;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	bool isDebugCameraActive_ = false;
	// ハンドル
	uint32_t textureHandle_ = 0;
	//音楽データ
	uint32_t soundDataHandle_ = 0;
	uint32_t deathSEHandle_ = 0;
	//音声再生ハンドル
	uint32_t voiceHandle_ = 0;
	uint32_t deathSEvoiceHandle_ = 0;
	//トランスフォーム

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
	// フェーズの切り替え
	void ChangePhase();
	// 死亡による終了フラグ
	bool deathFinished_ = false;
	// クリアによる終了フラグ
	bool clearFinished_ = false;
	// ステージ選択に戻るフラグ
	bool backSelect_ = false;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
};
