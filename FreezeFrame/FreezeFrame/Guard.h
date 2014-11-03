#pragma once

#include "Actor.h"

class FreezeFrame;

namespace guardNS
{
	const float SPEED=100;
	const COLOR_ARGB COLOR = graphicsNS::RED;
};

class Guard:public Actor
{
public:
	FreezeFrame *game;
	Actor targetEntity;
	bool target;

public:
	Guard();
	~Guard();

	bool initialize(FreezeFrame * game, int width, int height, int ncols, TextureManager *textureM);

	void update(float frametime);

	void ai(float time, Actor &t);

	void create(VECTOR2 loc);

	bool getHunting() {return target;}

	void vectorTrack(float time);
	void deltaTrack(float time);
	void evade(float time);

};