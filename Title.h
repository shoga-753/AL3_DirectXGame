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

class Title 
{
public:
	Title();
	~Title();

	void Initialize();
	void Update();
	void Draw2DNear();
};
