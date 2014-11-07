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

	timeUntilWallCollision = 1;

};
Player::~Player(){};

bool Player::initialize(FreezeFrame * g, Controls c, int width, int height, int ncols, TextureManager *playerTM, TextureManager *feetTM,TextureManager *cylinderTM)
{
	game = g;
	controls = c;
	
	bool result =  Actor::initialize(g,width,height,ncols,playerTM);
	result = result && feet.initialize(g,FEET_WIDTH,FEET_HEIGHT,FEET_COL,feetTM);
	feet.setFrames(0,2);
	feet.setCurrentFrame(2);
	feet.setFrameDelay(FEET_DELAY);
	feet.setLoop(true);
	feet.setCenter(getCenter());

	result = result && cylinder.initialize(g->getGraphics(),CYLINDER_WIDTH,CYLINDER_HEIGHT,CYLINDER_COLS,cylinderTM);
	cylinder.setCenter(CYLINDER_OFFSCREEN);
	cylinder.setCurrentFrame(CYLINDER_MAX_IMG_INDEX);

	return result;
}

void Player::draw(VECTOR2 screenLoc)
{
	if(getActive())
	{

		cylinder.draw(VECTOR2(0,0));

		feet.draw(screenLoc);

		Actor::draw(screenLoc);
	}
}

void Player::update(float &frametime)
{
	if(getActive())
	{
		if(alive)
		{

			VECTOR2 diffOn= CYLINDER_ONSCREEN-cylinder.getCenter();
			VECTOR2 diffOff= CYLINDER_OFFSCREEN-cylinder.getCenter();

			//if the gun ui needs to get in position
			if(numBullets > 0 && D3DXVec2LengthSq(&diffOn) > POS_EPSILON_SQRD)
			{
				D3DXVec2Normalize(&diffOn,&diffOn);
				cylinder.setCenter(cylinder.getCenter()+diffOn*CYLINDER_VERTICAL_SPEED*frametime);
			}
			//if the gun ui needs to get in position
			if(numBullets == 0 && D3DXVec2LengthSq(&diffOff) > POS_EPSILON_SQRD)
			{
				D3DXVec2Normalize(&diffOff,&diffOff);
				cylinder.setCenter(cylinder.getCenter()+diffOff*CYLINDER_VERTICAL_SPEED*frametime);
			}

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


			if(input->getMouseLButton()&& (weaponCooldown <= 0)&&numBullets>0){

				numBullets--;

				cylinder.setCurrentFrame(CYLINDER_MAX_IMG_INDEX-numBullets);

				cylinderDesiredDir = (CYLINDER_MAX_IMG_INDEX-numBullets)*PI/3;

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

			if(input->wasKeyPressed(controls.use))
			{
				Item* i = game->getItemUnderPlayer();
				if(i!=nullptr && i->getType()==Item::ItemType::WEAPON)
				{
					pickUpGun();
					i->setActive(false);
				}
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
			VECTOR2 endLoc = getCenter()+inputDir*currentSpeed*frametime;

			//efficiency, lets only do the expensive getPlayerRealEndLoc if we are moving
			if(endLoc != getCenter())
			{
				endLoc.x = max(0,min(game->getCurrentWorldSize().x,endLoc.x));
				endLoc.y = max(0,min(game->getCurrentWorldSize().y,endLoc.y));

				endLoc = game->getRealEndLoc(getCenter(),endLoc);
			}

			setCenter(endLoc);
			feet.setCenter(endLoc);

		
			weaponCooldown -= frametime;
			if(weaponCooldown < 0) weaponCooldown =0;

			recoilCooldown -= frametime;
			if(recoilCooldown < 0)recoilCooldown = 0;


			float diffAngle = cylinderDesiredDir - cylinder.getRadians();
			if(diffAngle > ROTATION_EPSILON)
			{
				cylinder.setRadians(cylinder.getRadians()+CYLINDER_ROTATION_SPEED*frametime*getSign(cylinderDesiredDir));
			}


		}
		else
		{
			frametime *= DEATH_TM;
		}
	}
	Image::update(frametime);
}

void Player::pickUpGun()
{
	gunAnimation = true; 
	numBullets = playerNS::GUN_BULLET_CAPACITY;
	cylinderDesiredDir = 0;
	cylinder.setRadians(0);
	cylinder.setCurrentFrame(0);
}



void Player::set(VECTOR2 loc)
{
	cylinder.setCenter(CYLINDER_OFFSCREEN);
	alive = true;
	setCenter(loc);
	cylinderDesiredDir = 0;
	cylinder.setRadians(0);
}