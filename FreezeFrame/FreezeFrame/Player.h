#pragma once

#include"Actor.h"
#include"Controls.h"
#include"constants.h"

class FreezeFrame;

namespace playerNS{
	const float RUN_SPEED = 300;
	const float WALK_SPEED = 300;
	const float WEAPON_COOLDOWN = 1;
	const VECTOR2 bulletDisplacement(28,-18);
	const float RECOIL_TIME = 0.25;

	const float D_TIME_PER_FRAME = 0.045;
	const float RUN_TM = 1;
	const float WALK_TM = 0.3;
	const float STANDING_TM = 0.01;
	const float RECOIL_TM = 1;

	const float TIME_EPSILON = 0.03;
}

class Player: public Actor
{
private:
	Controls controls;
	FreezeFrame* game;
	float weaponCooldown;
	float recoilCooldown;
	float currentTimeMultiplier;
	float desiredTimeMultiplier;

public:
	Player();
	~Player();

	bool initialize(FreezeFrame * game, Controls c, int width, int height, int ncols, TextureManager *textureM);

	void update(float &frameTime);

};