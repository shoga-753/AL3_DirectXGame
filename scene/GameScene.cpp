#include "GameScene.h"
#include "TextureManager.h"
#include"MathUtilityForText.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	//背景
	delete spriteBG_;
	//ステージ
	delete modelStage_;
	//自機
	delete modelPlayer_;
	//ビーム
	delete modelBeam_;

}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
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


}

void GameScene::Update() 
{ 
	playerUpdate();
	beamUpdate();
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
	
	spriteBG_->Draw();
	

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
	// ステージ
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);

	//自機表示
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);

	// ビーム
	if (beamFrag_ == true) 
	{
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
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

void GameScene::beamUpdate() 
{
	beamMove();
	beamBorn();

	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_, worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	worldTransformBeam_.TransferMatrix();
}
