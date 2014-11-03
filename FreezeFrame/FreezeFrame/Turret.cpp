#include "Turret.h"

#include "freezeframe.h"

Turret::Turret():Actor()
{
	edge.top = -32;
	edge.bottom = 32;
	edge.left = -32;
	edge.right = 76;

	collisionType = BOX;
	base.setRadians(PI/4);

	setActive(false);
	weaponCooldown = 0;
}

bool Turret::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *turretTM, TextureManager *baseTM)
{
	game = g;
	bool result =  Actor::initialize(g,width,height,ncols,turretTM);
	result = result && base.initialize(g,BASE_WIDTH,BASE_HEIGHT,0,baseTM);
	return result;
}

void Turret::update(float frametime)
{
	if(getActive())
	{
		VECTOR2 toPlayer = game->getPlayerLoc() - getCenter();
		float dirToPlayer = atan2(toPlayer.y,toPlayer.x);
		float distSqrdToPlayer = D3DXVec2LengthSq(&toPlayer);
		float radians = getRadians();

		//if the player is close and in view
		if(distSqrdToPlayer < turretNS::ENGAGE_DISTANCE_SQRD)
		{
			
			//convert to principle arguments
			dirToPlayer = toPincipleArgument(dirToPlayer);
			radians = toPincipleArgument(radians);

			float diff = dirToPlayer - radians;

			diff = toPincipleArgument(diff);

			//if we got um
			if(abs(diff) <= turretNS::ROT_EPSILON)
			{
				setRadians(dirToPlayer);
				radians = getRadians();
			}
			//rotate towards him
			else 
			{
				if (diff < 0 )
				{
					rotVel = -turretNS::ROTATION_SPEED;
				}
				else if (diff > 0)
				{
					rotVel = turretNS::ROTATION_SPEED;
				}
				setRadians(radians+ rotVel*frametime);
			}

			//fire gun, he's nearby
			weaponCooldown -= frametime;
			if(weaponCooldown <= 0)
			{
				game->spawnBullet(getCenter(),radians,graphicsNS::RED,false);
				animComplete = false;
				setCurrentFrame(0);
				weaponCooldown = turretNS::FIRE_RATE;
			}
		}
		else
		{
		
			if(radians > maxDir)
			{
				rotVel = -turretNS::ROTATION_SPEED;
			}
			if(radians < minDir)
			{
				setRadians(minDir);
				rotVel = turretNS::ROTATION_SPEED;
			}
			setRadians(radians+ rotVel*frametime);
		}
	}
	base.update(frametime);
	Image::update(frametime);


}

void Turret::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		

		base.draw(screenLoc);

		Actor::draw(screenLoc);
	}
}

void Turret::create(VECTOR2 loc, float dir)
{
	setActive(true);
	setRadians(dir);
	minDir = dir - turretNS::ROTATION_WIDTH;
	maxDir = dir + turretNS::ROTATION_WIDTH;
	rotVel = turretNS::ROTATION_SPEED;
	setCenter(loc);
	base.setCenter(loc);
}