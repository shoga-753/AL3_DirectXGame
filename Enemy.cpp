#include "Enemy.h"

Enemy::Enemy() {}

Enemy::~Enemy() 
{ 
	delete modelEnemy_;
}

void Enemy::Initialize(ViewProjection view) 
{ 
	viewProjection_ = view; 

	textureHandleEnemy_ = TextureManager::Load("enemy.png");
	modelEnemy_ = Model::Create();
	worldTransformEnemy_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformEnemy_.Initialize();
}

void Enemy::Update() 
{
	Move();
	Born();

	worldTransformEnemy_.matWorld_ = MakeAffineMatrix(
	    worldTransformEnemy_.scale_, worldTransformEnemy_.rotation_,
	    worldTransformEnemy_.translation_);

	worldTransformEnemy_.TransferMatrix();
}

void Enemy::Draw3D() 
{
	modelEnemy_->Draw(worldTransformEnemy_, viewProjection_, textureHandleEnemy_);
}

void Enemy::Move() 
{
	if (isAlive == 1) 
	{
		worldTransformEnemy_.rotation_.x -= 0.1f;

		worldTransformEnemy_.translation_.z -= 0.3f;
	}
	if (worldTransformEnemy_.translation_.z < -5.f) 
	{
		isAlive = 0;
	}
}

void Enemy::Born() 
{
	if (isAlive == false) {
		isAlive = true;
		worldTransformEnemy_.translation_.z = 40.f;

		int x = rand() % 80;
		float x2 = (float)x / 10 - 4;
		worldTransformEnemy_.translation_.x = x2;
	}
}
