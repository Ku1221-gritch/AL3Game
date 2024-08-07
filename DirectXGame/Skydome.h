#pragma once

#include "ViewProjection.h"
#include "WorldTransform.h"
#include "Model.h"

class Skydome {
	public:
	/// <summary>
	/// コンストラクタ
	/// </summary>
	Skydome();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~Skydome();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="textureHandle">テクスチャハンドル</param>
	void Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	///< summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection* viewProjection);

	private:
	//ワールド変換データ
	WorldTransform worldTransform_;
	//モデル
	Model* model_ = nullptr;
};
