#include "SelectScene.h"
#include "TextureManager.h"
#include <cassert>

SelectScene::SelectScene() {}

SelectScene::~SelectScene() {
	delete sprite_;
	delete modelBlock_;
	delete modelSkydome_;
	delete debugCamera_;
	delete modelPlayer_;
	delete deathParticles_;
	delete modelDeathParticle_;
	delete mapChipField_;
	delete modelMoveText_;
	delete modelJumpText_;
	delete modelBackText_;
	for (std::vector<WorldTransform*>& worldTransformBlockLine : worldTransformBlocks_) {
		for (WorldTransform* worldTransformBlock : worldTransformBlockLine) {
			delete worldTransformBlock;
		}
	}
	worldTransformBlocks_.clear();
}

// 初期化
void SelectScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	
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
	mapChipField_->LoadMapChipCsv("Resources/selectBlocks.csv");
	GenerateBlocks();

	// ビュープロジェクションの初期化
	viewProjection_.farZ = 200;
	viewProjection_.Initialize();

	// 座標をマップチップ番号で指定
	Vector3 playerPosition = mapChipField_->GetMapChipPositionByIndex(3, 18);

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

	// デバッグカメラの生成
	debugCamera_ = new DebugCamera(1280, 720);
	// カメラコントローラの初期化
	cameraController_ = new CameraController();
	cameraController_->Initialize();
	// 追従対象をセット
	cameraController_->SetTarget(player_);
	// 移動範囲の制限
	// 左右下上
	movableArea_ = {17.0f, 72.0f, 9.5f, 28.0f};
	cameraController_->SetMoveableArea(movableArea_);
	// リセット（瞬間合わせ）
	cameraController_->Reset();

	// ゴール
	modelGoal_ = Model::CreateFromOBJ("goal", true);
	Vector3 goalPosition = mapChipField_->GetMapChipPositionByIndex(30, 9);
	goal_ = new Goal();
	goal_->Initialize(modelGoal_, &viewProjection_, goalPosition);

	// 操作説明のモデル
	modelMoveText_ = Model::CreateFromOBJ("ugoku", true);
	const float kTextMove = 2.0f;
	worldTransformMoveText_.Initialize();
	worldTransformMoveText_.scale_ = {kTextMove, kTextMove, kTextMove};
	worldTransformMoveText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformMoveText_.translation_ = (0.0f, 0.0f, 22.0f);

	modelJumpText_ = Model::CreateFromOBJ("haneru", true);
	worldTransformJumpText_.Initialize();
	worldTransformJumpText_.scale_ = {kTextMove, kTextMove, kTextMove};
	worldTransformJumpText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformJumpText_.translation_ = (0.0f, 0.0f, 27.0f);
	
	modelBackText_ = Model::CreateFromOBJ("modoru", true);
	worldTransformBackText_.Initialize();
	worldTransformBackText_.scale_ = {kTextMove, kTextMove, kTextMove};
	worldTransformBackText_.rotation_.y = 0.99f * std::numbers::pi_v<float>;
	worldTransformBackText_.translation_ = (0.0f, 12.0f, 7.0f);
	
}

void SelectScene::Update() {

	//ウゴクとハネルの移動処理
	viewProjection_.TransferMatrix();
	worldTransformMoveText_.UpdateMatrix();
	counter_ += 1.0f / 60.0f;
	counter_ = std::fmod(counter_, kTimeTextMove);
	float angle = counter_ / kTimeTextMove * 2.0f * std::numbers::pi_v<float>;
	//ウゴク
	worldTransformMoveText_.translation_.x = std::sin(angle) + 17.0f;
	worldTransformMoveText_.translation_.y = 12.0f;
	//ハネル
	worldTransformJumpText_.UpdateMatrix();
	worldTransformJumpText_.translation_.y = std::sin(angle) + 12.0f;
	//モドル
	worldTransformBackText_.UpdateMatrix();
	worldTransformBackText_.translation_.y = 12.0f;
	worldTransformBackText_.translation_.z = std::sin(angle);
	

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

#ifdef _DEBUG
	if (input_->TriggerKey(DIK_Q)) {
		if (isDebugCameraActive_ == true) {
			isDebugCameraActive_ = false;
		} else if (isDebugCameraActive_ == false) {
			isDebugCameraActive_ = true;
		}
	}
#endif // _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		proceedStage1_ = true;
	}

	if (Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		backTitle_ = true;
	}
}

void SelectScene::Draw() {

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
	player_->Draw();

	modelMoveText_->Draw(worldTransformMoveText_, viewProjection_);
	modelJumpText_->Draw(worldTransformJumpText_, viewProjection_);
	modelBackText_->Draw(worldTransformBackText_, viewProjection_);


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

void SelectScene::GenerateBlocks() {
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

void SelectScene::CheckAllCollisions() {
#pragma region 自キャラと敵キャラの当たり判定
	{
		// 判定対象1と2の座標
		AABB aabb1, aabb3;

		// ゴールの座標
		aabb3 = goal_->GetAABB();
		// ゴールに自キャラが触れた時
		if (IsCollision(aabb1, aabb3)) {
			goal_->OnCollision(player_);
		}
	}
#pragma endregion
}

// AABB同士の交差判定
bool SelectScene::IsCollision(const AABB& aabb1, const AABB& aabb2) {
	if ((aabb1.min.x <= aabb2.max.x && aabb1.max.x >= aabb2.min.x) && (aabb1.min.y <= aabb2.max.y && aabb1.max.y >= aabb2.min.y) && (aabb1.min.z <= aabb2.max.z && aabb1.max.z >= aabb2.min.z)) {

		return true;
	}
	return false;
}