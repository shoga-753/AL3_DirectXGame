#include "GameScene.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	gameplay_ = new Gameplay();
	gameplay_->Initialize(viewProjection_);

	title_ = new Title();
	title_->Initialize();
}

void GameScene::Update() 
{ 
	switch (sceneMode_)
	{
case 0:
		gameplay_->Update();
		break;
case 1:
		title_->Update();
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
	switch (sceneMode_) 
	{
	case 0:
		gameplay_->Draw2DFar();
		break;
	case 1:
		title_->Draw2DNear();
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
	switch (sceneMode_) 
	{
	case 0:
		gameplay_->Draw3D();
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
	switch (sceneMode_) 
	{
	case 0:
		gameplay_->Draw2DNear();
		break;
	}

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}


