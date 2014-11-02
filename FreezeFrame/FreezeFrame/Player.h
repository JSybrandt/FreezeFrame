#pragma once

#include"Actor.h"
#include"Controls.h"
#include"constants.h"

class FreezeFrame;

namespace playerNS{
	const float SPEED = 200;
	const float WEAPON_COOLDOWN = 1;
}
class Player: public Actor
{
private:
	Controls controls;
	FreezeFrame* game;
	float weaponCooldown;

public:
	Player();
	~Player();

	bool initialize(FreezeFrame * game, Controls c, int width, int height, int ncols, TextureManager *textureM);

	void update(float &frameTime);

};