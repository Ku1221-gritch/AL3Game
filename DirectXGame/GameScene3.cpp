#include "GameScene3.h"
#include "TextureManager.h"
#include <cassert>

GameScene3::GameScene3() {}

GameScene3::~GameScene3() {
	delete sprite_;
	delete modelBlock_;
	delete modelSkydome_;
	delete debugCamera_;
	delete modelPlayer_;
	delete deathParticles_;
	delete modelDeathParticle_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformMapChip_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformMapChip_.clear();

	// 複数の敵のデリート
	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();

	// 複数の棘のデリート
	for (Needle* needle : needles_) {
		delete needle;
	}
	needles_.clear();

}

// 初期化
void GameScene3::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// マップチップで配置するオブジェクト
	modelBlock_ = Model::Create();

	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// スカイドーム
	modelSkydome_ = Model::CreateFromOBJ("stageskydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// マップチップフィールド
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/map/thirdStage.csv");
	GenerateBlocks();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();

	// 一旦敵停止
	//  敵キャラの生成
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);
	// 敵の位置
	for (int32_t i = 0; i < kEnemyMax; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(enemyPos[i].x, enemyPos[i].y);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
		enemies_.push_back(newEnemy);
		newEnemy->SetMapChipField(mapChipField_);
	}
	// 弾の位置
	bulletPosition_ = mapChipField_->GetMapChipPositionByIndex(25, 45);
	modelBullet_ = Model::CreateFromOBJ("enemyBullet", true);
	bullet_ = new Bullet();
	bullet_->Initialize(modelBullet_, &viewProjection_, bulletPosition_,bulletPosition_);

	// 棘の生成
	modelNeedle_ = Model::CreateFromOBJ("needle", true);

	// 棘の位置
	for (int i = 0; i < 20; ++i) {
		Needle* newNeedle = new Needle();
		Vector3 needlePosition = mapChipField_->GetMapChipPositionByIndex(needlePos[i].x, needlePos[i].y);
		newNeedle->Initialize(modelNeedle_, &viewProjection_, needlePosition);
		needles_.push_back(newNeedle);
		newNeedle->SetMapChipField(mapChipField_);
	}

	// 座標をマップチップ番号で指定
	// プレイヤーの初期位置
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(44, 3);

	// 自キャラの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();
	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// デスパーティクルの生成
	modelDeathParticle_ = Model::CreateFromOBJ("deathparticles", true);
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, playerPosition);

	// ゲームオーバーテキストの生成
	gameOverText_ = new GameOverText;
	gameOverText_->Initialize();
	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// カメラコントローラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// カメラの範囲制限（左,右,下,上）
	movableArea_ = {17.0f, 72.0f, 9.0f, 80.0f};
	cameraController_->SetMoveableArea(movableArea_);
	// リセット（瞬間合わせ）
	cameraController_->Reset();
	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;

	// ゴール
	modelGoal_ = Model::CreateFromOBJ("goal", true);
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(30, 9);
	goal_ = new Goal();
	goal_->Initialize(modelGoal_, &viewProjection_, goalPosition);
	// サウンドデータの読み込み
	soundDataHandle_ = audio_->LoadWave("music/op.wav");
	deathSEHandle_ = audio_->LoadWave("music/maou_se_battle02.wav");
	// 音楽再生
	voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
}

void GameScene3::Update() {

	ChangePhase();

	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		backSelect_ = true;
	}

	switch (phase_) {
	// ゲームプレイフェーズの処理
	case Phase::kPlay:
		cameraController_->Update();
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformMapChip_) {
			for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
				if (!worldTransformBlock)
					continue;

				worldTransformBlock->matWorld_ = MakeAffineMatrix(worldTransformBlock->scale_, worldTransformBlock->rotation_, worldTransformBlock->translation_); // アフィン変換行列

				// 定数バッファに転送する
				worldTransformBlock->TransferMatrix();
			}
		}

		// 自キャラの更新
		player_->Update();

		CheckAllCollisions();

		// スカイドームの更新処理
		skydome_->Update();

		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		// 棘の更新
		for (Needle* needle : needles_) {
			needle->Update();
		}

		// ゴールの更新
		goal_->Update();
		if (goal_->isGoal()) {
			clearFinished_ = true;
		}
		if (clearFinished_) {
			audio_->StopWave(voiceHandle_);
		}

		// カメラの処理
		if (isDebugCameraActive_) {
			// デバッグカメラの更新
			debugCamera_->Update();
			viewProjection_.matView = debugCamera_->GetViewProjection().matView;             // デバッグカメラのビュー行列;
			viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection; // デバッグカメラのプロジェクション行列;
			// ビュープロジェクション行列の転送
			viewProjection_.TransferMatrix();
		} else {
			viewProjection_.matView = cameraController_->GetViewProjection().matView;
			viewProjection_.matProjection = cameraController_->GetViewProjection().matProjection;
			viewProjection_.TransferMatrix();
		}

		break;
	case Phase::kDeath:

		deathFinished_ = true;

		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

		gameOverText_->Update();

#ifdef _DEBUG
		if (input_->TriggerKey(DIK_Q)) {
			if (isDebugCameraActive_ == true) {
				isDebugCameraActive_ = false;
			} else if (isDebugCameraActive_ == false) {
				isDebugCameraActive_ = true;
			}
		}

		// デスパーティクルの更新処理
		if (deathParticles_) {
			deathParticles_->Update();
		}

#endif // _DEBUG

		break;
	}
}

void GameScene3::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>

	// ブロックの描画
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformMapChip_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			if (!worldTransformBlock)
				continue;

			modelBlock_->Draw(*worldTransformBlock, viewProjection_);
		}
	}

	// スカイドームの描画処理
	skydome_->Draw(&viewProjection_);
	// プレイヤーの描画処理
	if (!player_->IsDead()) {
		player_->Draw();
		// 敵の描画処理
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}
	} else if (player_->IsDead()) {
		gameOverText_->Draw();
	}
	// 敵の描画処理
	for (Enemy* enemy : enemies_) {
		enemy->Draw();
	}
	// 棘の描画処理
	for (Needle* needle : needles_) {
		needle->Draw();
	}
	// ゴールの描画
	goal_->Draw();

	// デスパーティクルの描画処理
	if (deathParticles_) {
		deathParticles_->Draw();
	}

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene3::GenerateBlocks() {
	// 要素数
	uint32_t kNumMapChipVirtical = mapChipField_->GetNumMapChipVirtical();
	uint32_t kNumMapChipHorizontal = mapChipField_->GetNumMapChipHorizontal();
	// 要素数を変更する
	worldTransformMapChip_.resize(kNumMapChipVirtical);
	for (uint32_t i = 0; i < kNumMapChipVirtical; ++i) {
		worldTransformMapChip_[i].resize(kNumMapChipHorizontal);
	}
	// ブロックの生成
	for (uint32_t i = 0; i < kNumMapChipVirtical; ++i) {
		for (uint32_t j = 0; j < kNumMapChipHorizontal; ++j) {
			if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
				WorldTransform* worldTransform = new WorldTransform();
				worldTransform->Initialize();
				worldTransformMapChip_[i][j] = worldTransform;
				worldTransformMapChip_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
			}
		}
	}
}

void GameScene3::CheckAllCollisions() {
#pragma region プレイヤーの当たり判定
	{
		// 判定対象1と2の座標
		AABB aabb1, aabb2, aabb3, aabb4;

		// プレイヤーの座標
		aabb1 = player_->GetAABB();

		// プレイヤーと敵全ての当たり判定
		for (Enemy* enemy : enemies_) {
			// 敵の座標
			aabb2 = enemy->GetAABB();
			aabb4 = bullet_->GetAABB();

			// AABB同士の交差判定
			if (IsCollision(aabb1, aabb2)) {
				// プレイヤーの衝突時コールバックを呼び出す
				player_->OnCollision(enemy);
				// 敵の衝突時コールバックを呼び出す
				enemy->OnCollision(player_);
			}
			// 敵の弾との衝突判定
			if (IsCollision(aabb1, aabb4)) {
				//bullet_->OnCollision(player_);
				player_->OnCollision(enemy);
			}
		}
		// プレイヤーとゴールの当たり判定
		aabb3 = goal_->GetAABB();
		// ゴールにプレイヤーが触れた時
		if (IsCollision(aabb1, aabb3)) {
			// ゴールの衝突時コールバックを呼び出す
			goal_->OnCollision(player_);
		}
		// プレイヤーと棘全ての当たり判定
		for (Needle* needle : needles_) {
			// プレイヤーと棘の当たり判定
			aabb4 = needle->GetAABB();
			// 棘にプレイヤーが触れた時
			if (IsCollision(aabb1, aabb4)) {
				// プレイヤーの衝突時コールバックを呼び出す
				player_->OnCollision(needle);
				// 棘の衝突時コールバックを呼び出す
				needle->OnCollision(player_);
			}
		}
	}
#pragma endregion
}

// AABB同士の交差判定
bool GameScene3::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}
	return false;
}

void GameScene3::ChangePhase() {
	switch (phase_) {
	case Phase::kPlay:
		if (player_->IsDead()) {
			// 死亡演出
			phase_ = Phase::kDeath;

			const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			deathParticles_ = new DeathParticles;

			deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
			// 死亡SE
			deathSEvoiceHandle_ = audio_->PlayWave(deathSEHandle_);
			// 音楽停止
			audio_->StopWave(voiceHandle_);
		}
		break;
	case Phase::kDeath:

		break;
	}
}