#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "PrimitiveDrawer.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete sprite_;
	delete modelBlock_;
	delete modelSkydome_;
	delete debugCamera_;
	delete modelPlayer_;
	delete deathParticles_;
	delete modelDeathParticle_;
	delete mapChipField_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();

	for (Enemy* enemy : enemies_) {
		delete enemy;
	}
	enemies_.clear();
}

// 初期化
void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	// 音声
	//  textureHandle_ = TextureManager::Load("sample.png");
	//  soundDataHandle_ = audio_->LoadWave("mokugyo.wav");
	//  audio_->PlayWave(soundDataHandle_);
	//  voiceHandle_ = audio_->PlayWave(soundDataHandle_, true);
	//  sprite_ = Sprite::Create(textureHandle_, {100, 50});

	// ブロック
	modelBlock_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();

	// スカイドーム
	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_);

	// マップチップフィールド
	mapChipField_ = new MapChipField;
	mapChipField_->LoadMapChipCsv("Resources/blocks.csv");
	GenerateBlocks();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();

	// 敵キャラの生成
	modelEnemy_ = Model::CreateFromOBJ("Enemy", true);

	for (int32_t i = 5; i < 10; ++i) {
		Enemy* newEnemy = new Enemy();
		Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(6 * i, 18);
		newEnemy->Initialize(modelEnemy_, &viewProjection_, enemyPosition);

		enemies_.push_back(newEnemy);
		newEnemy->SetMapChipField(mapChipField_);
	}

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(1, 18);
	Vector3 enemyPosition = mapChipField_->GetMapChipPositionByIndex(12, 18);

	// 自キャラの生成
	modelPlayer_ = Model::CreateFromOBJ("player", true);
	player_ = new Player();

	// 自キャラの初期化
	player_->Initialize(modelPlayer_, &viewProjection_, playerPosition);
	player_->SetMapChipField(mapChipField_);

	// デスパーティクルの生成
	modelDeathParticle_ = Model::CreateFromOBJ("deathparticles", true);

	// 仮の生成処理。後で消す。
	deathParticles_ = new DeathParticles;
	deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, playerPosition);

	// 敵キャラの初期化
	// enemy_->SetMapChipField(mapChipField_);
	// enemy_->Initialize(modelEnemy_, &viewProjection_, enemyPosition);
	//  ライン描画が参照するビュープロジェクションを指定する（アドレス渡し）
	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);
	debugCamera_ = new DebugCamera(1280, 720);
	// 軸方向表示の表示を有効にする
	AxisIndicator::GetInstance()->SetVisible(true);
	// 軸方向表示が参照するビュープロジェクションを指定する（アドレス渡し）
	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
	movableArea_ = {0.0f, 150.0f, 0.0f, 100.0f};
	// カメラコントローラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// 移動範囲の指定
	cameraController_->SetMoveableArea(movableArea_);
	// リセット（瞬間合わせ）
	cameraController_->Reset();
	// ゲームプレイフェーズから開始
	phase_ = Phase::kPlay;
}

void GameScene::Update() {

	ChangePhase();

	switch (phase_) {
	// ゲームプレイフェーズの処理
	case Phase::kPlay:
		cameraController_->Update();
		// ブロックの更新
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
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


		////スプライトの今の座標を取得
		// Vector2 position = sprite_->GetPosition();
		////座標を｛2，1｝移動
		// position.x += 2.0f;
		// position.y += 1.0f;
		////移動した座標をスプライトに反映
		// sprite_->SetPosition(position);
		// スペースキーを押した瞬間
		// if (input_->TriggerKey(DIK_SPACE)) {
		//	//音声停止
		//	audio_->StopWave(voiceHandle_);
		//

		break;
	case Phase::kDeath:

		if (deathParticles_ && deathParticles_->IsFinished()) {
			finished_ = true;
		}
		// 敵キャラの更新
		for (Enemy* enemy : enemies_) {
			enemy->Update();
		}

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

#ifdef DEBUG
		// if (input_->TriggerKey())
		//  デバッグテキストの表示
		ImGui::Begin("Debug1");
		// float3入力ボックス
		ImGui::InputFloat3("InputFloat3", inputFloat3);
		// float3スライダー
		ImGui::SliderFloat3("SliderFloat3", inputFloat3, 0.0f, 1.0f);
		// デモウィンドウの表示を有効化
		ImGui::ShowDemoWindow();
		ImGui::End();
#endif // DEBUG
		break;
	}
}

	void GameScene::Draw() {

		// コマンドリストの取得
		ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
		// 背景スプライト描画前処理
		Sprite::PreDraw(commandList);

		/// <summary>
		/// ここに背景スプライトの描画処理を追加できる
		/// </summary>
		// ラインを描画する
		// PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

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
		for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
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
	    }
		// 敵の描画処理
		for (Enemy* enemy : enemies_) {
			enemy->Draw();
		}
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
		// sprite_->Draw();

		// スプライト描画後処理
		Sprite::PostDraw();

#pragma endregion
	}

	void GameScene::GenerateBlocks() {
		// 要素数
		uint32_t kNumBlockVirtical = mapChipField_->GetNumBlockVirtical();
		uint32_t kNumBlockHorizontal = mapChipField_->GetNumBlockHorizontal();
		// 要素数を変更する
		worldTransformBlocks_.resize(kNumBlockVirtical);
		for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
			worldTransformBlocks_[i].resize(kNumBlockHorizontal);
		}
		// ブロックの生成
		for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {
			for (uint32_t j = 0; j < kNumBlockHorizontal; ++j) {
				if (mapChipField_->GetMapChipTypeByIndex(j, i) == MapChipType::kBlock) {
					WorldTransform* worldTransform = new WorldTransform();
					worldTransform->Initialize();
					worldTransformBlocks_[i][j] = worldTransform;
					worldTransformBlocks_[i][j]->translation_ = mapChipField_->GetMapChipPositionByIndex(j, i);
				}
			}
		}
	}

	void GameScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定
		{
			// 判定対象1と2の座標
			AABB aabb1, aabb2;

			// 自キャラの座標
			aabb1 = player_->GetAABB();

			// 自キャラと敵全ての当たり判定
			for (Enemy* enemy : enemies_) {
				// 敵の座標
				aabb2 = enemy->GetAABB();

				// AABB同士の交差判定
				if (IsCollision(aabb1, aabb2)) {
					// 自キャラの衝突時コールバックを呼び出す
					player_->OnCollision(enemy);
					// 敵の衝突時コールバックを呼び出す
					enemy->OnCollision(player_);
				}
			}
		}
#pragma endregion
	}

	// AABB同士の交差判定
	bool GameScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
		if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

			return true;
		}
		return false;
	}

	void GameScene::ChangePhase() {
	    switch (phase_) {
	    case Phase::kPlay:
		    if (player_->IsDead()) {
			    // 死亡演出
			    phase_ = Phase::kDeath;

			    const Vector3& deathParticlesPosition = player_->GetWorldPosition();

			    deathParticles_ = new DeathParticles;

			    deathParticles_->Initialize(modelDeathParticle_, &viewProjection_, deathParticlesPosition);
		    }
		    break;
	    case Phase::kDeath:

		    break;
	    }
    }