#include "guard.h"
#include"freezeframe.h"

Guard::Guard():Actor(){}
Guard::~Guard(){}

bool Guard::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	return Actor::initialize(game,width,height,ncols,textureM);
}


void Guard::update(float frametime)
{
	//TODO: more than run right
	//setX(getX()+guardNS::SPEED*frametime);
	Actor::update(frametime);
}