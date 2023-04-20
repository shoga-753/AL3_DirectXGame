#include "GameScene.h"
#include"ImGuiManager.h"
#include "TextureManager.h"
#include"PrimitiveDrawer.h"
#include"AxisIndicator.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() 
{
	delete sprite_; 
	delete model_;
	delete debugCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();

	textreHandle_ = TextureManager::Load("sample.png");

	sprite_ = Sprite::Create(textreHandle_, {10, 10});

	modeltextreHandle_ = TextureManager::Load("sample.png");

	model_ = Model::Create();

	//ワールドトランスフォーム初期化
	worldTransform_.Initialize();
	//ビュープロジェクション初期化
	viewProjection_.Initialize();

	//音楽
	soundHandle_ = audio_->LoadWave("fanfare.wav");

	audio_->PlayWave(soundHandle_,true);

	PrimitiveDrawer::GetInstance()->SetViewProjection(&viewProjection_);

	debugCamera_ = new DebugCamera(WinApp::kWindowWidth, WinApp::kWindowHeight);

	AxisIndicator::GetInstance()->SetVisible(true);

	AxisIndicator::GetInstance()->SetTargetViewProjection(&debugCamera_->GetViewProjection());
}

void GameScene::Update() 
{ Vector2 pos = sprite_->GetPosition();
	pos.x += 1;
	pos.y += 1;
	sprite_->SetPosition(pos);

	if (input_->TriggerKey(DIK_SPACE))
	{
		audio_->StopWave(soundHandle_);
	}

	ImGui::ShowDemoWindow();

	ImGui::Begin("Test");
	ImGui::Text("kamata tarou %d.%d.%d", 2050, 12, 31);

	ImGui::InputFloat3("InputFloat3", inputFloat);

	ImGui::SliderFloat3("SliderFloat3", inputFloat, 0.0f, 1.0f);
	ImGui::End();

	debugCamera_->Update();
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
	sprite_->Draw();

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
	//モデル描画
	model_->Draw(worldTransform_, debugCamera_->GetViewProjection(), modeltextreHandle_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

	//ライン描画
	PrimitiveDrawer::GetInstance()->DrawLine3d({0, 0, 0}, {0, 10, 0}, {1.0f, 0.0f, 0.0f, 1.0f});

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
