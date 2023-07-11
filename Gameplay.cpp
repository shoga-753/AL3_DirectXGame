#include "Gameplay.h"
#include "TextureManager.h"
#include <cassert>

Gameplay::Gameplay() {}

Gameplay::~Gameplay() 
{
	delete stage_;
	delete player_;
	delete beam_;
	delete enemy_;
}

void Gameplay::Initialize(ViewProjection view) 
{ 
	

	viewProjection_.translation_.y = 1;
	viewProjection_.translation_.z = -6;
	viewProjection_.Initialize();

	stage_ = new Stage();
	stage_->Initialize(viewProjection_);

	player_ = new Player();
	player_->Initialize(viewProjection_);

	beam_ = new Beam();
	beam_->Initialize(viewProjection_, player_);

	enemy_ = new Enemy();
	enemy_->Initialize(viewProjection_);

	debugText_ = DebugText::GetInstance();
	debugText_->Initialize();
}

void Gameplay::Update() 
{
	stage_->Update();
	player_->Update();
	beam_->Update();
	enemy_->Update();

	CollisionPlayerEnemy();
	CollisionBeamEnemy();
}

void Gameplay::Draw2DFar() 
{
	stage_->Draw2DFar();
}

void Gameplay::Draw3D() 
{
	stage_->Draw3D();
	player_->Draw3D();
	beam_->Draw3D();
	enemy_->Draw3D();
}

void Gameplay::Draw2DNear() 
{
	char str[100];
	sprintf_s(str, "SCORE %d", gameScore);
	debugText_->Print(str, 200, 10, 2);

	char life[100];
	sprintf_s(life, "LIFE %d", playerLife);
	debugText_->Print(life, 500, 10, 2);
	debugText_->DrawAll();
}

void Gameplay::CollisionPlayerEnemy() {
	if (enemy_->GetFlag() == 1) {
		float dx = abs(player_->GetX() - enemy_->GetX());
		float dz = abs(player_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1) {
			enemy_->Hit();
			playerLife -= 1;
		}
	}
}

void Gameplay::CollisionBeamEnemy() {
	if (enemy_->GetFlag() == 1) {
		float dx = abs(beam_->GetX() - enemy_->GetX());
		float dz = abs(beam_->GetZ() - enemy_->GetZ());

		if (dx < 1 && dz < 1) {
			enemy_->Hit();
			beam_->Hit();
			gameScore += 10;
		}
	}
}



