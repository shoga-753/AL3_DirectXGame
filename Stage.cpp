#include "Stage.h"
#include"TextureManager.h"



Stage::Stage() {}

Stage::~Stage() 
{
	delete spriteBG_;
	delete modelStage_;
}

void Stage::Initialize(ViewProjection view)
{ textureHandleBG_ = TextureManager::Load("bg.jpg");
	spriteBG_ = Sprite::Create(textureHandleBG_, {0, 0});

	viewProjection_ = view;

	textureHandleStage_ = TextureManager::Load("stage.jpg");
	modelStage_ = Model::Create();
	worldTransformStage_.Initialize();

	worldTransformStage_.translation_ = {0, -1.5f, 0};
	worldTransformStage_.scale_ = {4.5f, 1, 40};

	worldTransformStage_.matWorld_ = MakeAffineMatrix(
	    worldTransformStage_.scale_, 
		worldTransformStage_.rotation_,
	    worldTransformStage_.translation_);

	worldTransformStage_.TransferMatrix();
}

void Stage::Update() {}

void Stage::Draw2DFar() 
{ spriteBG_->Draw(); }

void Stage::Draw3D() 
{
	modelStage_->Draw(worldTransformStage_, viewProjection_, textureHandleStage_);
}
