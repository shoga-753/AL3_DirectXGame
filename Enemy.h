#pragma once
#include "Audio.h"
#include "DirectXCommon.h"
#include "Input.h"
#include "Model.h"
#include "SafeDelete.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include"MathUtilityForText.h"

#include "Input.h"
#include"Player.h"

class Enemy 
{
public:
	Enemy();
	~Enemy();

	void Initialize(ViewProjection view);
	void Update();
	void Draw3D();

	void Move();
	void Born();

	int GetFlag() { return isAlive; }

	float GetX() { return worldTransformEnemy_.translation_.x; }
	float GetZ() { return worldTransformEnemy_.translation_.z; }

	void Hit() { isAlive = 0; }

	private:

		ViewProjection viewProjection_;

	    uint32_t textureHandleEnemy_ = 0;
	    Model* modelEnemy_ = nullptr;
	    WorldTransform worldTransformEnemy_;

	    Input* input_ = nullptr;

		int isAlive = 0;
};
