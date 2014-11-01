#include "Particle.h"

Particle::Particle()
{
	currentOpacity = 0;
	active = false;
	dissipationRate = 2;
}

void Particle::update(float frametime)
{
	if(active)
	{
		currentOpacity -= dissipationRate*frametime;
		if(currentOpacity<=0) active = false;

	}
}

void Particle::draw()
{
	if(active)
	{
		COLOR_ARGB c = graphicsNS::WHITE;
		if(currentOpacity < .25)
			c = graphicsNS::ALPHA25;
		else if(currentOpacity < .50)
			c = graphicsNS::ALPHA50;

		Actor::draw(c);
	}
}


void Particle::set(D3DXVECTOR2 loc)
{
	active = true;
	currentOpacity = 1;
	setCenterX(loc.x);setCenterY(loc.y);
}