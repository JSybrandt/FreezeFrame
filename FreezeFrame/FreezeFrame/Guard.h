#pragma once

#include "Actor.h"

class FreezeFrame;

namespace guardNS
{
	const float SPEED=1000;
};

class Guard:public Actor
{
public:
	FreezeFrame *game;

public:
	Guard();
	~Guard();

	bool initialize(FreezeFrame * game, int width, int height, int ncols, TextureManager *textureM);

	void update(float frametime);

};