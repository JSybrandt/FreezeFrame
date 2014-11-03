#include"Player.h"
#include"freezeframe.h"

Player::Player():Actor(){};
Player::~Player(){};

bool Player::initialize(FreezeFrame * g, Controls c, int width, int height, int ncols, TextureManager *textureM)
{
	game = g;
	controls = c;
	return Actor::initialize(game,width,height,ncols,textureM);
}

void Player::update(float &frametime)
{
	if(getActive())
	{
		VECTOR2 distToMouse = game->getMouseInWorld()-getCenter();
		float mouseDir = atan2(distToMouse.y,distToMouse.x);
		setRadians(mouseDir);

		VECTOR2 inputDir(0,0);

		if(input->isKeyDown(controls.up))
			inputDir.y=-1;
		if(input->isKeyDown(controls.down))
			inputDir.y=1;
		if(input->isKeyDown(controls.left))
			inputDir.x=-1;
		if(input->isKeyDown(controls.right))
			inputDir.x=1;

		if(input->getMouseLButton()&& (weaponCooldown <= 0) ){
			game->spawnBullet(getCenter()+utilityNS::rotateVector(playerNS::bulletDisplacement,mouseDir),mouseDir,getColorFilter(),true);
			weaponCooldown  = playerNS::WEAPON_COOLDOWN;
		}

		D3DXVec2Normalize(&inputDir,&inputDir);

		inputDir *= playerNS::SPEED*frametime;

		setCenter(getCenter()+inputDir);

		if(inputDir == VECTOR2(0,0)) {
			frametime *= 0.01;
		}
		

		weaponCooldown -= frametime;
		if(weaponCooldown < 0) weaponCooldown =0;

	}
}
