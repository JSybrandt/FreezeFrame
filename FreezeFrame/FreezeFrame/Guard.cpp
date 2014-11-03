#include "guard.h"
#include"freezeframe.h"

Guard::Guard():Actor(){
	edge.top = -32;
	edge.bottom = 32;
	edge.left = -12;
	edge.right = 12;
	collisionType = COLLISION_TYPE::BOX;
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
		if(health <= 0)
			setActive(false);

		setCenter(getCenter()+(getVelocity()*guardNS::SPEED*frameTime));
		setRadians(atan2(velocity.y,velocity.x));
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
	targetEntity = t;
	vectorTrack(time);
	//deltaTrack(time);
	//evade();
	return;
}