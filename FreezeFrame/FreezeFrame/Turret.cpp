#include "Turret.h"

#include "freezeframe.h"

Turret::Turret():Actor()
{
	edge.top = -32;
	edge.bottom = 32;
	edge.left = -32;
	edge.right = 76;

	collisionType = BOX;

	setActive(false);
	weaponCooldown = 0;
}

bool Turret::initialize(FreezeFrame * g, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	return Actor::initialize(g,width,height,ncols,textureM);
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
			
			//TODO:radian fix!
			if(dirToPlayer < 0)dirToPlayer+=2*PI;
			if(radians < 0) radians+= 2*PI;

			if(abs(radians - dirToPlayer) <= turretNS::ROT_EPSILON)
			{
				setRadians(dirToPlayer);
				radians = getRadians();
			}
			else 
			{
				if (radians > dirToPlayer)
				{
					rotVel = -turretNS::ROTATION_SPEED;
				}
				else if (radians < dirToPlayer)
				{
					rotVel = turretNS::ROTATION_SPEED;
				}
				setRadians(radians+ rotVel*frametime);
			}
			weaponCooldown -= frametime;
			if(weaponCooldown <= 0)
			{
				game->spawnBullet(getCenter(),radians,graphicsNS::RED,false);
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

}

void Turret::create(VECTOR2 loc, float dir)
{
	setActive(true);
	setRadians(dir);
	minDir = dir - turretNS::ROTATION_WIDTH;
	maxDir = dir + turretNS::ROTATION_WIDTH;
	rotVel = turretNS::ROTATION_SPEED;
	setCenter(loc);
}