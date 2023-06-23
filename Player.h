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

#include"Input.h"

class Player
{
public:
	Player();
	~Player();

	void Initialize(ViewProjection view);
	void Update();
	void Draw3D();

	float GetX() { return worldTransformPlayer_.translation_.x; }
	float GetZ() { return worldTransformPlayer_.translation_.z; }

private:
	ViewProjection viewProjection_;

	uint32_t textureHandlePlayer_ = 0;
	Model* modelPlayer_ = nullptr;
	WorldTransform worldTransformPlayer_;

	Input* input_ = nullptr;

	
};
