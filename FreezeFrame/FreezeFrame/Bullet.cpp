#include"Bullet.h"
#include "freezeframe.h"

bool Bullet::initialize(FreezeFrame* g, int width, int height, int ncols,TextureManager *textureM)
{
	game = g;
	setActive(false);
	return Actor::initialize(g,width,height,ncols,textureM);
}

void Bullet::create(VECTOR2 loc, float dir, COLOR_ARGB c)
{
	cooldown = 0;
	velocity = utilityNS::rotateVector(VECTOR2(1,0),dir)*bulletNS::SPEED;
	setCenter(loc);
	setColorFilter(c);
	setActive(true);
	firedLocation = loc;
	setRadians(dir);

}

void Bullet::update(float frametime)
{
	if(getActive())
	{
		if(cooldown <= 0)
		{
			game->spawnSmokeParticle(getCenter(),getColorFilter());
			cooldown = bulletNS::SMOKE_COOLDOWN;
		}
		else
			cooldown -= frametime;

		setCenter(getCenter()+velocity*frametime);
	}
}

void Bullet::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		//TODO: draw line from fired location to current location
		//VECTOR2 line[] = {firedLocation - screenLoc, getCenter()-screenLoc};
		//graphics->drawLine(line[0],line[2],getColorFilter());
		Actor::draw(screenLoc);
	}
}