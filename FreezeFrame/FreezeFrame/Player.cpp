#include"Player.h"
#include"freezeframe.h"

using namespace playerNS;

Player::Player():Actor(){
	//TODO: tighter collision
	/*edge.top = -32;
	edge.bottom = 32;
	edge.left = -12;
	edge.right = 12;
	collisionType = BOX;*/

	radius = 10;
	collisionType = CIRCLE;

	currentTimeMultiplier = 1;
	desiredTimeMultiplier = STANDING_TM;

};
Player::~Player(){};

bool Player::initialize(FreezeFrame * g, Controls c, int width, int height, int ncols, TextureManager *playerTM, TextureManager *feetTM)
{
	game = g;
	controls = c;
	
	bool result =  Actor::initialize(g,width,height,ncols,playerTM);
	result = result && feet.initialize(g,FEET_WIDTH,FEET_HEIGHT,FEET_COL,feetTM);
	feet.setFrames(0,2);
	feet.setCurrentFrame(2);
	feet.setFrameDelay(FEET_DELAY);
	feet.setLoop(true);


	return result;
}

void Player::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{
		feet.draw(screenLoc);

		Actor::draw(screenLoc);
	}
}

void Player::update(float &frametime)
{
	if(getActive())
	{
		float currentSpeed = playerNS::RUN_SPEED;
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
		if(inputDir != VECTOR2(0,0)) {
			feet.update(frametime);
			feet.setRadians(atan(inputDir.y/inputDir.x));
		}
		sin(inputDir.y);


		if(input->getMouseLButton()&& (weaponCooldown <= 0) ){

			//because we dont want to use the angle form player center
			VECTOR2 bulletLoc = getCenter()+utilityNS::rotateVector(playerNS::bulletDisplacement,mouseDir);
			VECTOR2 bulletPath = game->getMouseInWorld()-bulletLoc;
			float bulletAngle = atan2(bulletPath.y,bulletPath.x);

			game->spawnBullet(bulletLoc,bulletAngle,getColorFilter(),true);
			weaponCooldown  = playerNS::WEAPON_COOLDOWN;
			recoilCooldown = playerNS::RECOIL_TIME;
			animComplete = false;
			setCurrentFrame(0);
		}


		if(recoilCooldown > 0)
		{
			desiredTimeMultiplier = RECOIL_TM;
		}
		else if(inputDir == VECTOR2(0,0))
		{
			desiredTimeMultiplier = STANDING_TM;
		}
		//if walking
		else if(input->isKeyDown(controls.walk))
		{
			currentSpeed = WALK_SPEED;
			desiredTimeMultiplier = WALK_TM;
		}
		//if running
		else if(inputDir != VECTOR2(0,0))
		{
			currentSpeed = RUN_SPEED;
			desiredTimeMultiplier = RUN_TM;
		}
		
		


		if(currentTimeMultiplier - desiredTimeMultiplier < TIME_EPSILON)
			currentTimeMultiplier = desiredTimeMultiplier;

		if(currentTimeMultiplier != desiredTimeMultiplier)
		{
			float diff = desiredTimeMultiplier - currentTimeMultiplier;
			diff /= abs(diff);
			currentTimeMultiplier += diff*D_TIME_PER_FRAME;
		}

		frametime *= currentTimeMultiplier;


		D3DXVec2Normalize(&inputDir,&inputDir);
		inputDir *= currentSpeed*frametime;					
		setCenter(getCenter()+inputDir);
		feet.setCenter(getCenter() + inputDir);

		weaponCooldown -= frametime;
		if(weaponCooldown < 0) weaponCooldown =0;

		recoilCooldown -= frametime;
		if(recoilCooldown < 0)recoilCooldown = 0;

	}
	Image::update(frametime);
}
