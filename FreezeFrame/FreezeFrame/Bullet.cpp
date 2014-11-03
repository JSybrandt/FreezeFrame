#include"Bullet.h"
#include "freezeframe.h"

Bullet::Bullet(){
	radius = 8;
	collisionType = CIRCLE;
}

bool Bullet::initialize(FreezeFrame* g, int width, int height, int ncols,TextureManager *bulletTM, TextureManager* lineTM)
{
	game = g;
	setActive(false);
	bool result =  Actor::initialize(g,width,height,ncols,bulletTM);
	result = result && line.initialize(game->getGraphics(),0,0,0,lineTM);
	return result;
}

void Bullet::create(VECTOR2 loc, float dir, COLOR_ARGB c)
{
	velocity = utilityNS::rotateVector(VECTOR2(1,0),dir)*bulletNS::SPEED;
	setCenter(loc);
	setColorFilter(c);
	setActive(true);
	firedLocation = loc;
	setRadians(dir);

	line.setRadians(dir);

	line.setCenter(getCenter());
	
	VECTOR2 offset(line.getWidth()/2,0);
	offset = utilityNS::rotateVector(offset,dir);

	line.setCenter(line.getCenter()+offset);

	line.setColorFilter(c);

}

void Bullet::update(float frametime)
{
	if(getActive())
	{
		setCenter(getCenter()+velocity*frametime);
	}
}

void Bullet::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		VECTOR2 lineData = getCenter()-firedLocation;
		float lineLength = D3DXVec2Length(&lineData);
		RECT lineRect = line.getSpriteDataRect();
		lineRect.right = lineLength;
		line.setSpriteDataRect(lineRect);

		line.draw(screenLoc);

		Actor::draw(screenLoc);
	}
}