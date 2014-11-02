#include "guard.h"
#include"freezeframe.h"

Guard::Guard():Actor(){}
Guard::~Guard(){}

bool Guard::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	return Actor::initialize(game,width,height,ncols,textureM);
}


void Guard::update(float frameTime)
{
	//VECTOR2 foo = velocity*frameTime*guardNS::SPEED;
	//TODO: more than run right
	setX(getX()+velocity.x);
	setY(getY()+velocity.y);
	//incPosition(foo);
	Actor::update(frameTime);
}

void Guard::evade(float time)
{
	//add code here
	float newX = guardNS::SPEED*time;
	float newY = guardNS::SPEED*time;

	if(targetEntity.getCenterX() > getCenterX())
		newX = -newX;
	if(targetEntity.getCenterY() > getCenterY())
		newY = -newY;

	VECTOR2 test(newX, newY);
	D3DXVec2Normalize(&test, &test);
	setVelocity(test);
	return;
}

void Guard::deltaTrack(float time)
{
	//add code here
	float newX = 1;
	float newY = 1;

	if(targetEntity.getCenterX() < getCenterX())
		newX = -newX;
	if(targetEntity.getCenterY() < getCenterY())
		newY = -newY;

	VECTOR2 test(newX, newY);
	D3DXVec2Normalize(&test, &test);
	test *= guardNS::SPEED*game->timeMultiplier;
	
	setVelocity(test);

}
void Guard::vectorTrack(float time)
{
	
	VECTOR2 test = targetEntity.getCenter();
	test -= getCenter();
	D3DXVec2Normalize(&test,&test);
	test.x *= guardNS::SPEED*game->timeMultiplier;
	test.y *= guardNS::SPEED*game->timeMultiplier;
	setVelocity(test);
}

void Guard::ai(float time, Actor &t)
{ 
	targetEntity = t;
	//vectorTrack(time);
	deltaTrack(time);
	//evade();
	return;
}