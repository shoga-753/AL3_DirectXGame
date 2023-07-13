#include "GameScene.h"
#include "TextureManager.h"
#include"MathUtilityForText.h"
#include"time.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	//タイトル
	delete spriteTitle_;
	// タイトル(エンター)
	delete spriteEnter_;
	//ゲームオーバー
	delete spriteGameOver_;
	//背景
	delete spriteBG_;
	//ステージ
	delete modelStage_;
	//自機
	delete modelPlayer_;
	//ビーム
	delete modelBeam_;
	//敵機
	delete modelEnemy_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	//タイトル(2Dスプライト)
	textureHandleTitle_ = TextureManager::Load("title.png");
	spriteTitle_ = Sprite::Create(textureHandleTitle_, {0, 0});

	// タイトルエンター(2Dスプライト)
	textureHandleEnter_ = TextureManager::Load("enter.png");
	spriteEnter_ = Sprite::Create(textureHandleEnter_, {420, 420});

	// ゲームオーバー
	textureHandleGameOver_ = TextureManager::Load("gameover.png");
	spriteGameOver_ = Sprite::Create(textureHandleGameOver_, {0, 0});

	//背景
	textureHamdleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHamdleBG_, {0, 0});

	//ステージ
	viewProjection_.Initialize();

	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	//視点変更
	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	//ステージ位置変更
	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, 
		worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	worldTransformStage_.TransferMatrix();

	//自機表示
	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();
	//ビーム
	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.3f, 0.3f, 0.3f};
	worldTransformBeam_.Initialize();

	// 敵機表示
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();

	srand((unsigned int)time(NULL));

	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();

}

void GameScene::Update() 
{ 
	switch (sceneMode_) 
	{
	case 0://ゲームプレイ
		GamePlayUpdate();
		
		break;
	case 1:
		TitleUpdate();
		gamePlayStart();
		break;
	case 2:
		GameOverUpdate();
		break;
	}
	

}

void GameScene::Draw() {

	// コマンドリストの取得
	//背景
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

	


#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>
	switch (sceneMode_) 
	{
	case 0: // ゲームプレイ
		GamePlayDraw2DBack(); 
		break;
	case 2:
		GamePlayDraw2DBack();
		break;
	}
	
	

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
	switch(sceneMode_) 
	{
	case 0: // ゲームプレイ
		GamePlayDraw3D();
		break;
	case 2:
		GamePlayDraw3D();
		break;
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
	//debugText_->Print("AAA", 10, 10, 2);
	switch (sceneMode_) 
	{
	case 0: // ゲームプレイ
		GamePlayDraw2DNear(); 
		break;
	case 1:
		TitleDraw2DNear();
		break;
	case 2:
		GameOverDraw2DNear();
		break;
	}
	
	

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::GamePlayUpdate() 
{
	
	if (playerLife_<=0)
	{
		sceneMode_ = 2;

		playerLife_ = 3;
		
	}

	playerUpdate();
	beamUpdate();
	enemyUpdate();
	collision();
	collisionPlayerEnemy();
	collisionBeamEnemy();
}

void GameScene::GamePlayDraw3D() 
{
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	// 自機表示
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	if (beamFrag_ == true) {
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
	// 敵機表示
	if (enemyFrag_ == true) {
		modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
	}
}

void GameScene::GamePlayDraw2DBack() 
{ 
	spriteBG_->Draw(); 
}

void GameScene::GamePlayDraw2DNear() 
{
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore_);
	debugText_->Print(str, 200, 10, 2);

	sprintf_s(str, "LIFE %d", playerLife_);
	debugText_->Print(str, 10, 10, 2);
	debugText_->DrawAll();
}

void GameScene::TitleUpdate() 
{ 
	if (input_->TriggerKey(DIK_RETURN))
	{
		sceneMode_ = 0;
	}
	
	gameTimer_++; 
}

void GameScene::TitleDraw2DNear() 
{
	//タイトル表示
	spriteTitle_->Draw();

	//エンター表示
	if (gameTimer_ % 40 >= 20)
	{
		spriteEnter_->Draw();
	}
	
}

void GameScene::GameOverUpdate() 
{
	if (input_->TriggerKey(DIK_RETURN)) {
		sceneMode_ = 1;
	}

	gameTimer_++;
}

void GameScene::GameOverDraw2DNear() {
	spriteGameOver_->Draw();

	// エンター表示
	if (gameTimer_ % 40 >= 20) {
		spriteEnter_->Draw();
	}

}

void GameScene::playerUpdate() 
{
	//移動
	//右移動
	if (input_->PushKey(DIK_RIGHT))
	{
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	//左移動
	if (input_->PushKey(DIK_LEFT)) {
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4);
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4);


	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	worldTransformPlayer_.TransferMatrix();

	
}

void GameScene::beamMove() 
{ 
	if (beamFrag_ == true) 
	{
		worldTransformBeam_.rotation_.x += 0.1f;

		worldTransformBeam_.translation_.z += 0.1f;

		//画面外
		if (worldTransformBeam_.translation_.z >= 40.f)
		{
			beamFrag_ = false;
		}
			
	}
}

void GameScene::beamBorn() 
{
	if (input_->TriggerKey(DIK_SPACE)&&beamFrag_==false) 
	{

		worldTransformBeam_.translation_.x = worldTransformPlayer_.translation_.x;
		worldTransformBeam_.translation_.z = worldTransformPlayer_.translation_.z;

		beamFrag_ = true;
	}
}

void GameScene::enemyUpdate() 
{
	enemyMove();
	enemyBorn();

	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	worldTransformEnemy_.TransferMatrix();
}

void GameScene::enemyMove() 
{ 
	if (enemyFrag_ == true) 
	{

		worldTransformEnemy_.rotation_.x -= 0.1f;

		worldTransformEnemy_.translation_.z -= 0.3f;
	}
	if (worldTransformEnemy_.translation_.z < -5.f)
	{
		enemyFrag_ = false;
	}
	
}

void GameScene::enemyBorn() 
{
	if (enemyFrag_ == false)
	{
		enemyFrag_ = true;
		worldTransformEnemy_.translation_.z = 40.f;

		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
    }
}

void GameScene::collision() 
{
	collisionPlayerEnemy();
	collisionBeamEnemy();
}

void GameScene::collisionPlayerEnemy() 
{
	if (enemyFrag_==true)
	{
		float dx = abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_.translation_.z);

		if (dx < 1 && dz < 1)
		{
			enemyFrag_ = 0;
			playerLife_ -= 1;
		}
	}
}

void GameScene::collisionBeamEnemy() 
{
	if (enemyFrag_ == true&&beamFrag_==true) {
		float dx = abs(worldTransformBeam_.translation_.x - worldTransformEnemy_.translation_.x);
		float dz = abs(worldTransformBeam_.translation_.z - worldTransformEnemy_.translation_.z);

		if (dx < 1 && dz < 1) {
			enemyFrag_ = false;
			beamFrag_ = false;
			gameScore_ += 10;
		}
	}
}

void GameScene::gamePlayStart() 
{
	GamePlayUpdate();
	worldTransformPlayer_.translation_.x = 0;
	worldTransformEnemy_.translation_.x = 0;
	worldTransformBeam_.translation_.x = 0;
	worldTransformBeam_.translation_.z = 0;
	gameScore_ = 0;
	
}

void GameScene::beamUpdate() 
{
	beamMove();
	beamBorn();

	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	worldTransformBeam_.TransferMatrix();
}
