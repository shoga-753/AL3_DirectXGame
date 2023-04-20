#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugCamera.h"

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

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	
	/// </summary>

	uint32_t textreHandle_ = 0;

	Sprite* sprite_ = nullptr;

	//モデル
	uint32_t modeltextreHandle_ = 0;

	Model* model_ = nullptr;

	WorldTransform worldTransform_;

	ViewProjection viewProjection_;

	//音楽
	uint32_t soundHandle_;

	//デバックテキスト
	float inputFloat[3] = {0, 0, 0};

	DebugCamera* debugCamera_ = nullptr;
};
