#include "guard.h"
#include"freezeframe.h"

Guard::Guard():Actor(){
	edge.top = -32;
	edge.bottom = 32;
	edge.left = -12;
	edge.right = 12;
	collisionType = COLLISION_TYPE::BOX;
	colorFilter = guardNS::COLOR;
	setActive(false);
	target = false;
}
Guard::~Guard(){}

bool Guard::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	return Actor::initialize(game,width,height,ncols,textureM);
}


void Guard::update(float frameTime)
{
	if(getActive())
	{
		if(health <= 0) {
			setActive(false);
			audio->playCue(KILL1_CUE);
		}

		setCenter(getCenter()+(getVelocity()*guardNS::SPEED*frameTime));
		setRadians(atan2(velocity.y,velocity.x));
		if(velocity != VECTOR2(0,0))
			Actor::update(frameTime);
	}
}

void Guard::evade(float frameTime)
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(-disp);
}

void Guard::deltaTrack(float frametime)
{

	VECTOR2 v(0,0);

	if(targetEntity.getCenterX() < getCenterX())
		v.x = -1;
	if(targetEntity.getCenterX() > getCenterX())
		v.x = 1;

	if(targetEntity.getCenterY() < getCenterY())
		v.y = -1;
	if(targetEntity.getCenterY() > getCenterY())
		v.y = 1;

	D3DXVec2Normalize(&v, &v);
	setVelocity(v);

}
void Guard::vectorTrack(float frametime)
{
	VECTOR2 disp = targetEntity.getCenter()-getCenter();
	D3DXVec2Normalize(&disp,&disp);
	setVelocity(disp);
}

void Guard::ai(float time, Actor &t)
{ 
	VECTOR2 toPlayer = game->getPlayerLoc() - getCenter();
	float distSqrdToPlayer = D3DXVec2LengthSq(&toPlayer);

	if(distSqrdToPlayer < guardNS::ENGAGE_DISTANCE_SQRD) {
		target = true;
	}

	if(target) {
		targetEntity = t;
		vectorTrack(time);
		//deltaTrack(time);
		//evade();
	}
	return;
}

void Guard::create(VECTOR2 loc)
{
	target = false;
	velocity = VECTOR2(0,0);
	setActive(true);
	setCenter(loc);
}