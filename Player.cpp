#include "Player.h"

Player::Player() {}

Player::~Player() 
{
	delete modelPlayer_;
}

void Player::Initialize(ViewProjection view) 
{ 
	
	viewProjection_ = view;

	textureHandlePlayer_ = TextureManager::Load("player.png");
	modelPlayer_ = Model::Create();
	worldTransformPlayer_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformPlayer_.Initialize();

	input_ = Input::GetInstance();

}

void Player::Update() 
{
	 worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, 
		worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	worldTransformPlayer_.TransferMatrix();

	// 移動
	// 右移動
	if (input_->PushKey(DIK_RIGHT)) 
	{
		worldTransformPlayer_.translation_.x += 0.1f;
	}
	// 左移動
	if (input_->PushKey(DIK_LEFT)) 
	{
		worldTransformPlayer_.translation_.x -= 0.1f;
	}
	worldTransformPlayer_.translation_.x = max(worldTransformPlayer_.translation_.x, -4);
	worldTransformPlayer_.translation_.x = min(worldTransformPlayer_.translation_.x, 4);

	worldTransformPlayer_.matWorld_ = MakeAffineMatrix(
	    worldTransformPlayer_.scale_, worldTransformPlayer_.rotation_,
	    worldTransformPlayer_.translation_);

	worldTransformPlayer_.TransferMatrix();
}

void Player::Draw3D() 
{
	modelPlayer_->Draw(worldTransformPlayer_, viewProjection_, textureHandlePlayer_);
}
