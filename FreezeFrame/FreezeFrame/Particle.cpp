#include "Particle.h"

Particle::Particle():Actor()
{
	setActive(false);
}

void Particle::update(float frametime)
{
	if(active)
	{
		age += frametime;
		if(age>=particleNS::LIFETIME) active = false;

	}
}

void Particle::draw(VECTOR2 screenLoc)
{
	if(active)
	{
		COLOR_ARGB c = Graphics::calculateAlpha(1-(age/particleNS::LIFETIME))&colorFilter;
		Actor::draw(screenLoc,c);
	}
}


void Particle::create(D3DXVECTOR2 loc,COLOR_ARGB c)
{

	active = true;
	age = 0;
	setCenterX(loc.x);setCenterY(loc.y);
	setColorFilter(c);

}