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

class Beam 
{
public:
	Beam();
	~Beam();

	void Initialize(ViewProjection view, Player* player);
	void Update();
	void Draw3D();

	void Move();
	void Born();

	float GetX() { return worldTransformBeam_.translation_.x; }
	float GetZ() { return worldTransformBeam_.translation_.z; }

	void Hit() { isAlive = 0; }

private:
	ViewProjection viewProjection_;

	uint32_t textureHandleBeam_ = 0;
	Model* modelBeam_ = nullptr;
	WorldTransform worldTransformBeam_;

	Input* input_ = nullptr;
	int isAlive = 0;

	Player* player_ = nullptr;
};
