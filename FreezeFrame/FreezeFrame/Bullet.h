#pragma once

#include"Actor.h"

class FreezeFrame;

namespace bulletNS{
	const float SPEED = 1000;
	const float SMOKE_COOLDOWN=0.001;
};

class Bullet: public Actor
{
private:
	FreezeFrame* game;
	float cooldown; //used when spawning smoke
	VECTOR2 firedLocation;
public:

	bool initialize(FreezeFrame* g, int width, int height, int ncols,TextureManager *textureM);

	void create(VECTOR2 loc, float dir,COLOR_ARGB c);

	void update(float frametime);

	void draw(VECTOR2 screenLoc);

};