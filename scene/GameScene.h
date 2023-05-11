#pragma once

#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"DebugText.h"

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

	private:
		//自機更新
	    void beamUpdate();
	    void playerUpdate();
	    void beamMove();
	    void beamBorn();

	    void enemyUpdate();
	    void enemyMove();
	    void enemyBorn();

		void collision();
	    void collisionPlayerEnemy();
	    void collisionBeamEnemy();
	    

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;

	/// <summary>
	/// ゲームシーン用
	/// </summary>
	//背景
	uint32_t textureHamdleBG_ = 0;
	Sprite* spriteBG_ = nullptr;

	//ステージ
	ViewProjection viewProjection_;

	//
	uint32_t textureHandleStage_ = 0;
	Model* modelStage_ = nullptr;
	WorldTransform worldTransformStage_;

	// ビーム
	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;
	bool beamFrag_ = false;

	//自機
	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;
	//敵機
	uint32_t textureHandleEnemy_ = 0;
	Model* modelEnemy_ = nullptr;
	WorldTransform worldTransformEnemy_;
	bool enemyFrag_ = false;

	DebugText* debugText_ = nullptr;
	int gameScore_ = 0;
	int playerLife_ = 3;


};
