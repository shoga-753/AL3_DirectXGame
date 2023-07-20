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

	// サウンドデータの読み込み
	titleBGM_ = audio_->LoadWave("Audio/ring05.wav");
	gamePlayBGM_ = audio_->LoadWave("Audio/ring08.wav");
	gameoverBGM_ = audio_->LoadWave("Audio/ring09.wav");
	enemyHitBGM_ = audio_->LoadWave("Audio/chord.wav");
	playerHitBGM_ = audio_->LoadWave("Audio/tada.wav");

	voiceHandleBGM_ = audio_->PlayWave(titleBGM_, true);
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
	for (int  i = 0; i < 10; i++) 
	{
		worldTransformBeam_[i].scale_ = {0.3f, 0.3f, 0.3f};
		worldTransformBeam_[i].Initialize();
	}
	

	// 敵機表示
	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	for (int i = 0; i < 10; i++) 
	{
		worldTransformEnemy_[i].scale_ = {0.5f, 0.5f, 0.5f};
		worldTransformEnemy_[i].Initialize();
	}
	

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
		if (sceneMode_ != 1)
		{
			audio_->StopWave(voiceHandleBGM_);
			voiceHandleBGM_ = audio_->PlayWave(gameoverBGM_, true);
			sceneMode_ = 2;
		}
		
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
	for (int i = 0; i < 10; i++) 
	{
		if (beamFrag_[i] == true) {
			modelBeam_->Draw(worldTransformBeam_[i], viewProjection_, textureHandleBeam_);
		}
	}
	
	// 敵機表示
	for (int i = 0; i < 10; i++) 
	{
		if (enemyFrag_[i] == true) {
			modelEnemy_->Draw(worldTransformEnemy_[i], viewProjection_, textureHandleEnemy_);
		}
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
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(gamePlayBGM_, true);
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
	if (input_->TriggerKey(DIK_RETURN)) 
	{
		audio_->StopWave(voiceHandleBGM_);
		voiceHandleBGM_ = audio_->PlayWave(titleBGM_, true);
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
	for (int i = 0; i < 10; i++) 
	{
		if (beamFrag_[i] == true) {
			worldTransformBeam_[i].rotation_.x += 0.1f;

			worldTransformBeam_[i].translation_.z += 0.1f;

			// 画面外
			if (worldTransformBeam_[i].translation_.z >= 40.f) {
				beamFrag_[i] = false;
			}
		}
	}
	
}

void GameScene::beamBorn() {
	if (beamTimer_ == 0) {
		for (int i = 0; i < 10; i++) {
			if (input_->TriggerKey(DIK_SPACE) && beamFrag_[i] == false) {

				worldTransformBeam_[i].translation_.x = worldTransformPlayer_.translation_.x;
				worldTransformBeam_[i].translation_.z = worldTransformPlayer_.translation_.z;

				beamFrag_[i] = true;
			}
			if (rand() % 2 == 0) {
				enemySpeed_[i];
			}
		}
	} else {
		beamTimer_++;
		if (beamTimer_ > 10) {
			beamTimer_ = 0;
		}
	}
}
void GameScene::enemyUpdate() 
{
	enemyMove();
	enemyBorn();
	for (int i = 0; i < 10; i++) 
	{
		worldTransformEnemy_[i].matWorld_ = MakeAffineMatrix(
		    worldTransformEnemy_[i].scale_, worldTransformEnemy_[i].rotation_,
		    worldTransformEnemy_[i].translation_);

		worldTransformEnemy_[i].TransferMatrix();
	}
	
}

void GameScene::enemyMove() 
{ 
	for (int i = 0; i < 10; i++) 
	{
		if (enemyFrag_[i] == true) {

			worldTransformEnemy_[i].rotation_.x -= 0.1f;

			worldTransformEnemy_[i].translation_.x +=enemySpeed_[i] ;

			if (worldTransformEnemy_[i].translation_.x >= 4) {
				enemySpeed_[i] = -0.1f;
			}
			if (worldTransformEnemy_[i].translation_.x <= -4) {
				enemySpeed_[i] = 0.1f;
			}

			worldTransformEnemy_[i].translation_.z -= 0.3f;
		}
		if (worldTransformEnemy_[i].translation_.z < -5.f) {
			enemyFrag_[i] = false;
		}

	}
	
	
}

void GameScene::enemyBorn() 
{
	if (rand() % 10 == 0) 
	{
		for (int i = 0; i < 10; i++) {
			if (enemyFrag_[i] == false) {
				enemyFrag_[i] = true;
				worldTransformEnemy_[i].translation_.z = 40.f;

				if (rand() % 2 == 0) {
					enemySpeed_[i] = 0.1f;
				} else {
					enemySpeed_[i] = -0.1f;
				}

				int x = rand() % 80;
				float x2 = (float)x / 10 - 4;
				worldTransformEnemy_[i].translation_.x = x2;
				break;
			}
			
		}
		
			
		
	}
	
}

void GameScene::collision() 
{
	collisionPlayerEnemy();
	collisionBeamEnemy();
}

void GameScene::collisionPlayerEnemy() 
{
	for (int  i = 0; i < 10; i++) 
	{
		if (enemyFrag_[i] == true) {
			float dx =
			    abs(worldTransformPlayer_.translation_.x - worldTransformEnemy_[i].translation_.x);
			float dz =
			    abs(worldTransformPlayer_.translation_.z - worldTransformEnemy_[i].translation_.z);

			if (dx < 1 && dz < 1) {
				enemyFrag_[i] = 0;
				playerLife_ -= 1;
				audio_->PlayWave(playerHitBGM_);
			}
		}
	}
	
}

void GameScene::collisionBeamEnemy() 
{
	for (int e = 0; e < 10; e++) 
	{
		if (enemyFrag_[e] == true) 
		{
			for (int i = 0; i < 10; i++) 
			{
				if (beamFrag_[i] == true) 
				{
					float dx =
					    abs(worldTransformBeam_[i].translation_.x -
					        worldTransformEnemy_[e].translation_.x);
					float dz =
					    abs(worldTransformBeam_[i].translation_.z -
					        worldTransformEnemy_[e].translation_.z);

					if (dx < 1 && dz < 1) {
						enemyFrag_[e] = false;
						beamFrag_[i] = false;
						gameScore_ += 10;
						audio_->PlayWave(enemyHitBGM_);
					}
				}
			}
			
	}
	
	}
}

void GameScene::gamePlayStart() 
{
	
	worldTransformPlayer_.translation_.x = 0;
	for (int  i = 0; i < 10; i++) 
	{
	enemyFrag_[i] = 0;
	worldTransformEnemy_[i].translation_.x = 0;
	worldTransformEnemy_[i].translation_.z = 40;
	}
	for (int i = 0; i < 10; i++) 
	{
	worldTransformBeam_[i].translation_.x = 0;
	worldTransformBeam_[i].translation_.z = 0;
	}
	
	gameScore_ = 0;
	playerLife_ = 3;
	GamePlayUpdate();
}

void GameScene::beamUpdate() 
{
	beamMove();
	beamBorn();
	for (int i = 0; i < 10; i++) 
	{
	worldTransformBeam_[i].matWorld_ = MakeAffineMatrix(
		worldTransformBeam_[i].scale_, worldTransformBeam_[i].rotation_,
		worldTransformBeam_[i].translation_);

	worldTransformBeam_[i].TransferMatrix();
	}
	
}
