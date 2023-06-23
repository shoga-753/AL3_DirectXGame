#include "Beam.h"

Beam::Beam() {}

Beam::~Beam() 
{ 
	delete modelBeam_;
}

void Beam::Initialize(ViewProjection view, Player* player) {
	
	viewProjection_ = view;
	player_ = player;

	textureHandleBeam_ = TextureManager::Load("beam.png");
	modelBeam_ = Model::Create();
	worldTransformBeam_.scale_ = {0.5f, 0.5f, 0.5f};
	worldTransformBeam_.Initialize();

	input_ = Input::GetInstance();
}

void Beam::Update() 
{
	Move();
	Born();

	worldTransformBeam_.matWorld_ = MakeAffineMatrix(
	    worldTransformBeam_.scale_,
		worldTransformBeam_.rotation_,
	    worldTransformBeam_.translation_);

	worldTransformBeam_.TransferMatrix();
}

void Beam::Draw3D() 
{ 
	if (isAlive == 1) 
	{
		modelBeam_->Draw(worldTransformBeam_, viewProjection_, textureHandleBeam_);
	}
	
}

void Beam::Move() 
{
	if (isAlive==1)
	{
		worldTransformBeam_.translation_.z += 1.f;

		if (worldTransformBeam_.translation_.z>=40.f)
		{
			isAlive = 0;
		}
	}
}

void Beam::Born() 
{
	if (isAlive==0&&input_->TriggerKey(DIK_SPACE))
	{
		isAlive = 1;
		worldTransformBeam_.translation_.x = player_->GetX();
		worldTransformBeam_.translation_.z = 0.f;
	}
}
