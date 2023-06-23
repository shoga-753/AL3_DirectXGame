#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete stage_;
	delete player_;
	delete beam_;
	delete enemy_;
}



void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	stage_ = new Stage();
	stage_->Initialize(viewProjection_);

	player_ = new Player();
	player_->Initialize(viewProjection_);

	beam_ = new Beam();
	beam_->Initialize(viewProjection_,player_);

	enemy_ = new Enemy();
	enemy_->Initialize(viewProjection_);

	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

void GameScene::Update() 
{ 
	stage_->Update(); 
	player_->Update();
	beam_->Update();
	enemy_->Update();

	CollisionPlayerEnemy();
	CollisionBeamEnemy();
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
	stage_->Draw2DFar();

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
	stage_->Draw3D();
	player_->Draw3D();
	beam_->Draw3D();
	enemy_->Draw3D();

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore);
	debugText_->Print(str, 200, 10, 2);
	
	char life[100];
	sprintf_s(life, "LIFE %d", playerLife);
	debugText_->Print(life, 500, 10, 2);
	debugText_->DrawAll();

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CollisionPlayerEnemy() 
{
	if (enemy_->GetFlag()==1) 
	{
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1)
		{
			enemy_->Hit();
			playerLife -= 1;
		}
	}
}

void GameScene::CollisionBeamEnemy() 
{
	if (enemy_->GetFlag() == 1) 
	{
		float dx = abs(beam_->GetX() - enemy_->GetX());
		float dz = abs(beam_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1) 
		{
			enemy_->Hit();
			beam_->Hit();
			gameScore += 10;
		}
	}
}
