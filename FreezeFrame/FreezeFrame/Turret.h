#pragma once

#include "Actor.h"

class FreezeFrame;

namespace turretNS{
	const float ROTATION_SPEED = PI/4;
	const float ENGAGE_DISTANCE_SQRD = pow(1000,2); // replace this with "line of sight"
	const float ROTATION_WIDTH = PI/2;
	const float ROT_EPSILON = 0.001;
	const float FIRE_RATE = 0.5;
};

class Turret:public Actor
{
private:
	FreezeFrame * game;
	float minDir, maxDir,rotVel;
	float weaponCooldown;
public:
	Turret();

	bool initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM);

	void update(float frameTime);

	void animate(float frameTime);

	void create(VECTOR2 loc, float dir);

};