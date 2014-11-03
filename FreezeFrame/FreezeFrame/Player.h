#pragma once

#include"Actor.h"
#include"Controls.h"
#include"constants.h"

class FreezeFrame;

namespace playerNS{
	const float SPEED = 300;
	const float WEAPON_COOLDOWN = 1;
	const VECTOR2 bulletDisplacement(28,-18);
}
class Player: public Actor
{
private:
	Controls controls;
	FreezeFrame* game;
	float weaponCooldown;
	float timeMultiplier;

public:
	Player();
	~Player();

	bool initialize(FreezeFrame * game, Controls c, int width, int height, int ncols, TextureManager *textureM);

	void update(float &frameTime);

};