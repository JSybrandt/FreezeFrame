// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Game Engine Part 1
// Chapter 4 RedSpace.cpp v1.0
// RedSpace is the class we create.

#include "freezeframe.h"

//=============================================================================
// Constructor
//=============================================================================
FreezeFrame::FreezeFrame()
{
	srand(time(0));

	P1Controls = Controls('W','S','A','D','C','E');

	worldSizes = new VECTOR2[GameState::SIZE];
	worldSizes[GameState::TitleScreen] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);
	worldSizes[GameState::Level1] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);
	worldSizes[GameState::Level2] = VECTOR2(2048,4096);
	worldSizes[GameState::Level3] = VECTOR2(1300,2048);
	worldSizes[GameState::FeelingLucky] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);

	currentState = TitleScreen;

	paused = false;

	l2pCheat=false;
	infAmmoCheat=false;
	textCooldown = 0;
}

//=============================================================================
// Destructor
//=============================================================================
FreezeFrame::~FreezeFrame()
{
	releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void FreezeFrame::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError


	if(!backgroundTex.initialize(graphics,BACKGROUND_IMAGE))
		throw GameError(1,"Failed to init background tex");
	if(!manTex.initialize(graphics,MAN_IMAGE))
		throw GameError(2,"Failed to init man tex");
	if(!turretTex.initialize(graphics,TURRET_IMAGE))
		throw GameError(3,"Failed to init turret tex");
	if(!bulletTex.initialize(graphics,BULLET_IMAGE))
		throw GameError(4,"Failed to init bullet tex");
	if(!walkTex.initialize(graphics,WALK_IMAGE))
		throw GameError(5,"Failed to init walk tex");
	if(!feetTex.initialize(graphics,FEET_IMAGE))
		throw GameError(5,"Failed to init walk tex");
	if(!cursorTex.initialize(graphics,CURSOR_IMAGE))
		throw GameError(5,"Failed to init cursor tex");
	if(!particleTex.initialize(graphics,PARTICLE_IMAGE))
		throw GameError(6,"Failed to init particle tex");
	if(!menuTex.initialize(graphics,MENU_IMAGE))
		throw GameError(7,"Failed to init menu tex");
	if(!menuCursorTex.initialize(graphics,MENU_CURSOR_IMAGE))
		throw GameError(8,"Failed to init menu cursor tex");
	if(!lineTex.initialize(graphics,LINE_IMAGE))
		throw GameError(9,"Failed to init line tex");
	if(!exitTex.initialize(graphics,EXIT_IMAGE))
		throw GameError(9,"Failed to init exit tex");
	if(!wallTex.initialize(graphics,WALL_IMAGE))
		throw GameError(10,"Failed to init wall tex");
	if(!baseTex.initialize(graphics,BASE_IMAGE))
		throw GameError(9,"Failed to init exit tex");
	if(!mineTex.initialize(graphics,MINE_IMAGE))
		throw GameError(9,"Failed to init mine tex");
	if(!cylinderTex.initialize(graphics,CYLINDER_IMAGE))
		throw GameError(9,"Failed to init cylinder tex");
	if(!dangerZoneTex.initialize(graphics,DANGER_ZONE_IMAGE))
		throw GameError(9,"Failed to init danger zone tex");
	if(!infoText.initialize(graphics,20,true,false,"Courier New"))
		throw GameError(9,"Failed to init mine text");

	infoText.setFontColor(graphicsNS::BLACK);

	if(!gunTex.initialize(graphics,GUN_IMAGE))
		throw GameError(11,"Failed to init gun text");

	if(!exit.initialize(this,0,0,0,&exitTex))
		throw GameError(11,"Failed to init exit");

	if(!pauseTex.initialize(graphics,PAUSE_IMAGE))
		throw GameError(11,"Failed to init gun text");

	if(!pause.initialize(graphics,0,0,0,&pauseTex))
		throw GameError(11,"Failed to init pause");

	if(!title.initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(12,"Failed to init title");
	title.setCurrentFrame(MENU_TITLE);
	if(!subtitle.initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(13,"Failed to init subtitle");
	subtitle.setCurrentFrame(MENU_SUBTITLE);

	for(int i = 0; i < freezeFrameNS::NUM_MENU_OPTIONS; i++)
	{
		if(!menuItems[i].initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(-1*i,"Failed to init menu item");

		menuItems[i].setCurrentFrame(MENU_PLAY+i);
	}

	if(!menuCursor.initialize(graphics,0,0,0,&menuCursorTex))
		throw GameError(14,"Failed to init menu corsor");

	if(!background.initialize(graphics,0,0,0,&backgroundTex))
		throw GameError(15,"Failed to init background image");

	if(!player.initialize(this,P1Controls,&infoText,PL_WIDTH,PL_HEIGHT,PL_COL,&manTex, &feetTex,&cylinderTex))
		throw GameError(24,"Failed to init player");
	player.setFrames(0, 5);
	player.setCurrentFrame(5);
	player.setFrameDelay(PL_DELAY);
	player.setLoop(false);

	if(!cursor.initialize(this,0,0,0,&cursorTex))
		throw GameError(25,"Failed to init cursor");

	for(int i = 0; i < MAX_GUARDS; i++)
	{
		if(!guards[i].initialize(this,64,64,4,&walkTex))
			throw GameError(-1*i,"FAILED TO MAKE DUDE!");
		guards[i].setFrames(0, 6);   // animation frames
		guards[i].setCurrentFrame(0);     // starting frame
		guards[i].setFrameDelay(0.05f); //0.08 seems appriopriate
	}

	for(int i = 0; i < MAX_TURRETS; i++)
	{
		if(!turrets[i].initialize(this,TURRET_WIDTH,TURRET_HEIGHT,TURRET_COL,&turretTex, &baseTex))
			throw GameError(-1*i,"FAILED TO MAKE turret!");
		turrets[i].setFrames(0, 4);   // animation frames
		turrets[i].setCurrentFrame(4);     // starting frame
		turrets[i].setFrameDelay(TURRET_DELAY); //
		turrets[i].setLoop(false);
	}

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].initialize(this,0,0,0,&particleTex))
			throw GameError(-1*i,"FAILED TO MAKE particle!");
	}

	for(int i = 0 ; i < MAX_PLAYER_BULLETS; i++)
	{
		if(!playerBullets[i].initialize(this,0,0,0,&bulletTex,&lineTex))
			throw GameError(-1*i,"FAILED TO MAKE player bullet!");
	}

	for(int i = 0 ; i < MAX_ENEMY_BULLETS; i++)
	{
		if(!enemyBullets[i].initialize(this,0,0,0,&bulletTex,&lineTex))
			throw GameError(-1*i,"FAILED TO MAKE enemy bullet!");
	}

	for(int i = 0 ; i < MAX_WALLS; i++)
	{
		if(!walls[i].initialize(this,0,0,0,&wallTex))
			throw GameError(-1*i,"FAILED TO MAKE wall!");
	}

	for(int i = 0; i < MAX_MINES; i++)
	{
		if(!mines[i].initialize(this,&infoText,MINE_WIDTH,MINE_HEIGHT,MINE_COL,&mineTex,&dangerZoneTex))
			throw GameError(-1*i,"FAILED TO MAKE explosion!");
		mines[i].setFrames(0, 1);   // animation frames
		mines[i].setCurrentFrame(0);     // starting frame
		mines[i].setFrameDelay(MINE_DELAY); //
		mines[i].setLoop(true);
	}
	for(int i = 0; i < MAX_ITEMS; i++)
	{
		items[i].initialize(this,0,0,0,&gunTex);
	}

	player.setColorFilter(COLOR_ARGB(0xFF3E52ED));

	currentState = TitleScreen;
	menuLoad();
	
	ShowCursor(false);

	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void FreezeFrame::update()
{
	
		switch (currentState){
		case TitleScreen:
			menuUpdate();
			break;
		default:
			levelsUpdate();
		}
	
}

void FreezeFrame::menuUpdate(bool reset)
{
	static float counter = 0; 
	static int selectedItem = 0;

	if(reset){
		counter = 0;
		selectedItem = 0;
	}
	else
	{
		counter += frameTime;

		if(title.getX()>0)title.setX(0);
		if(title.getX()<0)title.setX(title.getX()+MENU_ITEM_SPEED*frameTime);

		if(counter > MENU_ITEM_DELAY){
			if(subtitle.getX()>0)subtitle.setX(0);
			if(subtitle.getX()<0)subtitle.setX(subtitle.getX()+MENU_ITEM_SPEED*frameTime);
		}


		for(int i = 0 ; i < freezeFrameNS::NUM_MENU_OPTIONS; i++)
		{
			if(counter > (i+2)*MENU_ITEM_DELAY)
			{
				if(menuItems[i].getX()<GAME_WIDTH-menuItems[i].getWidth())menuItems[i].setX(GAME_WIDTH-menuItems[i].getWidth());
				if(menuItems[i].getX()>GAME_WIDTH-menuItems[i].getWidth())menuItems[i].setX(menuItems[i].getX()-MENU_ITEM_SPEED*frameTime);
			}

			if(selectedItem==i)
			{
				menuCursor.setY(menuItems[i].getY());
			}

		}

		if(input->wasKeyPressed(P1Controls.up))
			selectedItem-=1;
		if(input->wasKeyPressed(P1Controls.down))
			selectedItem+=1;
		if(selectedItem < 0)
			selectedItem = 3;
		if(selectedItem > 3)
			selectedItem = 0;

		if(input->wasKeyPressed(VK_RETURN))
		{
			switch (selectedItem)
			{
			case 0:
				level1Load();
				break;
			case 1:
				feelingLuckyLoad();
				break;
			case 3:
				PostQuitMessage(0);
				break;
			default:
				break;
			}
		}

		if(textCooldown > 0) textCooldown -= frameTime;
		if(textCooldown < 0) textCooldown = 0;

		if(input->isKeyDown('L')&&input->isKeyDown('2')&&input->isKeyDown('P'))
		{
			statusString = "L2P N00B: Player Is Invincible!";
			textCooldown = TEXT_ON_SCREEN;
			l2pCheat = true;
		}

		if(input->isKeyDown('A')&&input->isKeyDown('M')&&input->isKeyDown('O'))
		{
			statusString = "AMMO: Infinite Ammo!";
			textCooldown = TEXT_ON_SCREEN;
			infAmmoCheat = true;
		}

	}
}

void FreezeFrame::levelsUpdate()
{
	if(input->wasKeyPressed(PAUSE_BUTTON))
	{
		paused = !paused;
		ShowCursor(paused);
	}

	if(paused) return;


	if(!player.alive)
	{
		playerDeathCountdown -= frameTime;
		if(playerDeathCountdown<=0)
			menuLoad();
	}

	worldFrameTime = frameTime;
	player.update(worldFrameTime);
	updateScreen(player.getCenter());


	cursor.update(worldFrameTime);

	for(int i = 0; i < MAX_GUARDS; i++)
	{
		guards[i].update(worldFrameTime);
		
	}

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		particles[i].update(worldFrameTime);
	}

	for(int i = 0 ; i < MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].update(worldFrameTime);
	}

	for(int i = 0 ; i < MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].update(worldFrameTime);
	}
	for(int i = 0; i < MAX_TURRETS; i++)
	{
		turrets[i].update(worldFrameTime);
	}
	for(int i = 0; i < MAX_MINES; i++)
	{
		mines[i].update(worldFrameTime);
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FreezeFrame::ai()
{
	for(int i = 0; i < MAX_GUARDS; i++) {
		guards[i].ai(worldFrameTime, player);
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void FreezeFrame::collisions()
{
	if(currentState!=TitleScreen)
	{
		VECTOR2 collisionVector;
		for(int i = 0; i < MAX_PLAYER_BULLETS; i++)
		{

			for(int j = 0 ; j < MAX_GUARDS; j++)
				if(playerBullets[i].collidesWith(guards[j],collisionVector))
				{
					guards[j].setHealth(0);
					spawnParticleCloud(guards[j].getCenter(),graphicsNS::RED);
					playerBullets[i].setActive(false);
				}

			for(int j = 0 ; j < MAX_TURRETS; j++)
				if(playerBullets[i].collidesWith(turrets[j],collisionVector))
				{
					turrets[j].hit();
					playerBullets[i].setActive(false);
				}

			for(int j = 0; j < MAX_WALLS; j++)
				if(playerBullets[i].collidesWith(walls[j],collisionVector))
				{
					playerBullets[i].setActive(false);
				}

			//leaves screen
			if(playerBullets[i].getCenter().x < 0 || playerBullets[i].getCenter().x > worldSizes[currentState].x || 
			   playerBullets[i].getCenter().y < 0 || playerBullets[i].getCenter().y > worldSizes[currentState].y)
			   playerBullets[i].setActive(false);
		}

		for(int i = 0; i < MAX_ENEMY_BULLETS; i++)
		{
			if(enemyBullets[i].getCenter().x < 0 || enemyBullets[i].getCenter().x > worldSizes[currentState].x || 
			   enemyBullets[i].getCenter().y < 0 || enemyBullets[i].getCenter().y > worldSizes[currentState].y)
			   enemyBullets[i].setActive(false);

			if(enemyBullets[i].collidesWith(player,collisionVector)) {
				onPlayerDeath();
			}

			for(int j = 0; j < MAX_WALLS; j++)
				if(enemyBullets[i].collidesWith(walls[j],collisionVector))
				{
					enemyBullets[i].setActive(false);
				}
		}

		for(int j = 0; j < MAX_WALLS; j++)
			if(player.collidesWith(walls[j],collisionVector))
			{
				//TODO: better cut off
				//player.setCenter(player.getCenter()-collisionVector);
			}

		for(int i = 0; i < MAX_MINES; i++)
		{
			//if a landmine blew up a player
			if(player.collidesWith(mines[i],collisionVector))
			{
				mines[i].wasSteppedOn();
				if(mines[i].getDangerous())
					onPlayerDeath(); //TODO:something cool on death
			}			
		}

		if(player.collidesWith(exit,collisionVector))
		{
			switch (currentState)
			{
			case FreezeFrame::Level1:
				level2Load();
				break;
			case FreezeFrame::Level2:
				level3Load();
				break;
			case FreezeFrame::Level3:
				menuLoad();
				break;
			case FreezeFrame::RestartScreen:
				break;
			case FeelingLucky:
				menuLoad();
			default:
				break;
			}
		}
	}

}

//=============================================================================
// Render game items
//=============================================================================
void FreezeFrame::render()
{
	graphics->spriteBegin();                // begin drawing sprites

	switch (currentState){
	case TitleScreen:
		menuRender();
		break;
	default:
		levelsRender();
	}

	graphics->spriteEnd();                  // end drawing sprites


}

void FreezeFrame::menuRender()
{
	VECTOR2 UIScreenLoc(0,0);

	background.draw(UIScreenLoc);

	title.draw(UIScreenLoc);
	subtitle.draw(UIScreenLoc);
	for(int i = 0 ; i < freezeFrameNS::NUM_MENU_OPTIONS; i++)
	{
		menuItems[i].draw(UIScreenLoc);
	}

	menuCursor.draw(UIScreenLoc);
	

	if(textCooldown > 0)
	{
		infoText.print(statusString,0,0);
	}

}

void FreezeFrame::levelsRender()
{
	background.draw(screenLoc);

	exit.draw(screenLoc);

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		particles[i].draw(screenLoc);
	}

	for(int i = 0 ; i < MAX_PLAYER_BULLETS; i++)
	{
		playerBullets[i].draw(screenLoc);
	}

	for(int i = 0 ; i < MAX_ENEMY_BULLETS; i++)
	{
		enemyBullets[i].draw(screenLoc);
	}

	for(int i = 0; i < MAX_GUARDS; i++)
	{
		guards[i].draw(screenLoc,graphicsNS::FILTER);
	}

	for(int i = 0; i < MAX_TURRETS; i++)
	{
		turrets[i].draw(screenLoc);
	}

	for(int i = 0; i < MAX_WALLS; i++)
	{
		walls[i].draw(screenLoc);
	}

	for(int i = 0; i < MAX_MINES; i++)
	{
		mines[i].draw(screenLoc);
	}
	for(int i = 0; i < MAX_ITEMS; i++)
	{
		items[i].draw(screenLoc);
	}

	player.draw(screenLoc);

	cursor.draw(screenLoc);

	if(paused)
		pause.draw(VECTOR2(0,0));

}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void FreezeFrame::releaseAll()
{
	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void FreezeFrame::resetAll()
{
	Game::resetAll();
	return;
}



void FreezeFrame::menuLoad()
{
	currentState = TitleScreen;
	deactivateAll();

	float padding = 20;
	title.setX(-title.getWidth());
	title.setY(padding);

	subtitle.setX(-subtitle.getWidth());
	subtitle.setY(title.getY()+title.getHeight()+padding);

	for(int i = 0 ; i < freezeFrameNS::NUM_MENU_OPTIONS; i++)
	{
		menuItems[i].setCenterY(worldSizes[currentState].y/2+((MENU_CELL_HEIGHT+padding)*i));
		menuItems[i].setX(worldSizes[currentState].x);
	}

	menuCursor.setCenterY(menuItems[0].getCenterY());
	menuCursor.setX(GAME_WIDTH-menuCursor.getWidth());

	menuUpdate(true);

}

void FreezeFrame::level1Load()
{
	currentState = Level1;
	deactivateAll();

	player.set(VECTOR2(worldSizes[currentState].x/6,100));

	VECTOR2 offset(30,-30);

	Turret * t1 = spawnTurret(VECTOR2(0,0),-PI/4);
	Turret * t2 = spawnTurret(VECTOR2(0,0),3*PI/4);

	t1->setLeft(0);
	t1->setBot(worldSizes[currentState].y);
	t1->setCenter(t1->getCenter()+offset);

	t2->setRight(worldSizes[currentState].x);
	t2->setTop(0);
	t2->setCenter(t2->getCenter()-offset);

	Wall* w1 = spawnWall(VECTOR2(0,0),VECTOR2(45,worldSizes[currentState].y*2/3));
	Wall* w2 = spawnWall(VECTOR2(0,0),VECTOR2(45,worldSizes[currentState].y*2/3));

	w1->setCenterX(worldSizes[currentState].x/3);

	w2->setCenterX(worldSizes[currentState].x*2/3);
	w2->setBot(worldSizes[currentState].y);

	spawnMine(worldSizes[currentState]*0.4);
	spawnMine(worldSizes[currentState]*0.5);
	spawnMine(worldSizes[currentState]*0.6);

	spawnItem(VECTOR2(worldSizes[currentState].x/6,300),Item::ItemType::WEAPON);

	exit.setBot(worldSizes[currentState].y);
	exit.setRight(worldSizes[currentState].x);
	exit.update(0);
}

void FreezeFrame::level2Load()
{
	currentState = Level2;
	deactivateAll();

	player.set(VECTOR2(100,100));


	spawnWall(VECTOR2(300,0),VECTOR2(250,600));
	spawnWall(VECTOR2(950,0),VECTOR2(600,180));
	spawnWall(VECTOR2(1150,0),VECTOR2(300,300));
	spawnWall(VECTOR2(550,450),VECTOR2(1100,150));
	spawnWall(VECTOR2(1450,600),VECTOR2(200,800));

	spawnWall(VECTOR2(0,900),VECTOR2(300,700));
	spawnWall(VECTOR2(0,1600),VECTOR2(1650,200));

	spawnWall(VECTOR2(worldSizes[currentState].x - 1650,2450),VECTOR2(1800,200));
	spawnWall(VECTOR2(650,2650),VECTOR2(1000,400));
	spawnWall(VECTOR2(650,3050),VECTOR2(200,850));

	spawnWall(worldSizes[currentState]-VECTOR2(550,550),VECTOR2(550,550));


	spawnItem(VECTOR2(750,100),Item::ItemType::WEAPON);
	spawnItem(VECTOR2(500,4000),Item::ItemType::WEAPON);

	spawnTurret(VECTOR2(1000,250),PI/2);

	spawnTurret(VECTOR2(1700,800),PI/2);

	spawnTurret(VECTOR2(1850,100),PI/2);

	spawnTurret(VECTOR2(1000,800),PI);
	spawnTurret(VECTOR2(600,1300),0);

	spawnTurret(VECTOR2(500,2700),PI);
	spawnTurret(VECTOR2(500,2900),PI);
	spawnTurret(VECTOR2(500,3100),PI);
	spawnTurret(VECTOR2(500,3300),PI);
	spawnTurret(VECTOR2(500,3500),PI);
	spawnTurret(VECTOR2(500,3700),PI);

	spawnGuard(VECTOR2(550,1900));
	spawnGuard(VECTOR2(150,2100));
	spawnGuard(VECTOR2(800,2375));

	spawnGuard(VECTOR2(1100,1900));
	spawnGuard(VECTOR2(1475,2100));
	spawnGuard(VECTOR2(975,2375));

	spawnGuard(VECTOR2(1000,3200));
	spawnGuard(VECTOR2(1875,3000));

	for(int i = 400; i < 1400; i+=200)
	{
		for(int j = 600; j < 1500; j+= 200)
		{
			spawnMine(VECTOR2(i,j));
		}
	}

	for(int i = 0; i < 250; i+=75)
	{
		for(int j = 2500; j < worldSizes[currentState].y; j+= 150)
		{
			spawnMine(VECTOR2(i,j));
		}
	}



	exit.setCenter(VECTOR2(1875,2760));
	exit.update(0);
	
}

void FreezeFrame::level3Load()
{
	currentState = Level3;
	deactivateAll();

	player.set(VECTOR2(worldSizes[currentState].x/6,1800));

	VECTOR2 offset(30,-30);

	/*Turret * t1 = spawnTurret(VECTOR2(0,0),-PI/4);
	Turret * t2 = spawnTurret(VECTOR2(0,0),3*PI/4);

	t1->setLeft(0);
	t1->setBot(worldSizes[currentState].y);
	t1->setCenter(t1->getCenter()+offset);

	t2->setRight(worldSizes[currentState].x);
	t2->setTop(0);
	t2->setCenter(t2->getCenter()-offset);*/

	Wall* w1 = spawnWall(VECTOR2(0,1600),VECTOR2(600,50));
	Wall* w2 = spawnWall(VECTOR2(800,1600),VECTOR2(600,50)); //Entrance way

	Wall* w3 = spawnWall(VECTOR2(0,2000),VECTOR2(worldSizes[currentState].x, 100)); //Bottom wall

	Wall* w4 = spawnWall(VECTOR2(1250,0),VECTOR2(50,worldSizes[currentState].y)); //Wall along right side
	Wall* w5 = spawnWall(VECTOR2(0,0),VECTOR2(50,worldSizes[currentState].y-448)); //Wall along right side
	Wall* w6 = spawnWall(VECTOR2(0,0),VECTOR2(50,worldSizes[currentState].y-448));

	Wall* w7 = spawnWall(VECTOR2(0,0),VECTOR2(50,worldSizes[currentState].y-448));

	//w1->setCenterX(worldSizes[currentState].x/3);

	//w2->setCenterX(worldSizes[currentState].x*2/3);
	//w2->setBot(worldSizes[currentState].y);

	spawnMine(VECTOR2(700,1625));
	//spawnMine(worldSizes[currentState]*0.5);
	//spawnMine(worldSizes[currentState]*0.6);

	//spawnItem(VECTOR2(worldSizes[currentState].x/6,300),Item::ItemType::WEAPON);

	exit.setBot(100);
	exit.setRight(worldSizes[currentState].x);
	exit.update(0);
}

void FreezeFrame::feelingLuckyLoad()
{

	currentState = FeelingLucky;
	deactivateAll();

	VECTOR2 playerPos(worldSizes[currentState]/2);
	VECTOR2 turretDisp(worldSizes[currentState].y/2,0);

	for(float i = 0 ; i < 2*PI; i+=PI/3)
	{
		spawnTurret(playerPos+rotateVector(turretDisp,i),i+PI);
	}

	player.set(playerPos);

	exit.setRight(worldSizes[currentState].x);
	exit.setBot(worldSizes[currentState].y);
	exit.update(0);
}


void FreezeFrame::updateScreen(VECTOR2 center)
{
	//TODO: nicer screen centering
	screenLoc = VECTOR2(center.x - GAME_WIDTH/2,center.y-GAME_HEIGHT/2);

	if(screenLoc.x<0)screenLoc.x=0;
	if(screenLoc.y<0)screenLoc.y=0;
	if(screenLoc.x+GAME_WIDTH>worldSizes[currentState].x)screenLoc.x=worldSizes[currentState].x-GAME_WIDTH;
	if(screenLoc.y+GAME_HEIGHT>worldSizes[currentState].y)screenLoc.y=worldSizes[currentState].y-GAME_HEIGHT;

}


Bullet* FreezeFrame::spawnBullet(VECTOR2 loc, float dir,COLOR_ARGB c, bool playerBullet)
{
	int maxBullets = MAX_ENEMY_BULLETS;
	Bullet * arr = enemyBullets;

	if(playerBullet)
	{
		maxBullets = MAX_PLAYER_BULLETS;
		arr = playerBullets;
	}

	for(int i = 0; i < maxBullets; i++)
	{
		if(!arr[i].getActive())
		{
			arr[i].create(loc,dir,c);
			return &arr[i];
		}
	}

	return nullptr;
}


Particle* FreezeFrame::spawnParticle(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].getActive())
		{
			particles[i].create(loc,vel,c);
			return &particles[i];
		}
	}

	return nullptr;
}

Turret* FreezeFrame::spawnTurret(VECTOR2 loc, float dir)
{
	for(int i = 0; i < MAX_TURRETS; i++)
	{
		if(!turrets[i].getActive())
		{
			turrets[i].create(loc,dir);
			return &turrets[i];
		}
	}

	return nullptr;
}

Guard* FreezeFrame::spawnGuard(VECTOR2 loc)
{
	for(int i = 0; i < MAX_GUARDS; i++)
	{
		if(!guards[i].getActive())
		{
			guards[i].create(loc);
			return &guards[i];
		}
	}

	return nullptr;
}

Wall* FreezeFrame::spawnWall(VECTOR2 loc, VECTOR2 size)
{
	for(int i = 0; i < MAX_WALLS; i++)
	{
		if(!walls[i].getActive())
		{
			walls[i].create(loc,size);
			return &walls[i];
		}
	}
	return nullptr;
}

LandMine* FreezeFrame::spawnMine(VECTOR2 loc)
{
	for(int i = 0; i < MAX_MINES; i++)
	{
		if(!mines[i].getActive())
		{
			mines[i].create(loc);
			return &mines[i];
		}
	}
	return nullptr;
}

Item* FreezeFrame::spawnItem(VECTOR2 loc, Item::ItemType t)
{
	for(int i = 0; i < MAX_ITEMS; i++)
	{
		if(!items[i].getActive())
		{
			items[i].create(loc,t);
			return &items[i];
		}
	}
	return nullptr;
}

Item* FreezeFrame::getItemUnderPlayer()
{
	VECTOR2 v;
	for(int i = 0; i < MAX_ITEMS; i++)
	{
		if(items[i].collidesWith(player,v))
		{
			return &items[i];
		}
	}
	return nullptr;
}

void FreezeFrame::spawnParticleCloud(VECTOR2 loc, COLOR_ARGB c)
{
	float dir,spd;
	VECTOR2 v(1,0);
	for(int i = 0; i < NUM_PARTICLES_IN_CLOUD_EFFECT; i++)
	{
		dir = rand01()*2*PI;
		spd = rand01()*particleNS::CLOUD_VEL;
		spawnParticle(loc,rotateVector(v,dir)*spd,c);
	}
}

void FreezeFrame::spawnParticleCone(VECTOR2 loc,float dir, COLOR_ARGB c)
{
	float currentdir,spd;
	VECTOR2 v(1,0);
	for(int i = 0; i < NUM_PARTICLES_IN_CONE_EFFECT; i++)
	{
		currentdir = rand01()*PI/4-PI/8+dir;
		spd = (rand01()/2+0.5)*particleNS::CONE_VEL;
		spawnParticle(loc,rotateVector(v,currentdir)*spd,c);
	}
}



void FreezeFrame::deactivateAll()
{
	for(int i = 0 ; i < MAX_GUARDS; i++)
		guards[i].setActive(false);
	for(int i = 0 ; i < MAX_TURRETS; i++)
		turrets[i].setActive(false);
	for(int i = 0 ; i < MAX_PLAYER_BULLETS; i++)
		playerBullets[i].setActive(false);
	for(int i = 0 ; i < MAX_ENEMY_BULLETS; i++)
		enemyBullets[i].setActive(false);
	for(int i = 0 ; i < MAX_PARTICLES; i++)
		particles[i].setActive(false);
	for(int i = 0; i < MAX_WALLS; i++)
		walls[i].setActive(false);
	for(int i = 0; i < MAX_MINES; i++)
		mines[i].setActive(false);
	for(int i = 0; i < MAX_ITEMS; i++)
		items[i].setActive(false);
}



VECTOR2 FreezeFrame::getRealEndLoc(VECTOR2 startLoc, VECTOR2 endLoc)
{
	for(int i = 0; i < MAX_WALLS; i++)
	{
		if(walls[i].getActive())
		{
			VECTOR2 wallTL(walls[i].getX(),walls[i].getY());
			VECTOR2 wallBR(walls[i].getX()+ walls[i].getWidth(),walls[i].getY()+walls[i].getHeight());

			VECTOR2 disp = endLoc - wallTL;
			if(disp.x >= 0 && disp.y >= 0 && disp.x < walls[i].getWidth() && disp.y < walls[i].getHeight())
			{
				if(startLoc.x <= wallTL.x)
				{
					endLoc.x = wallTL.x;
				}
				if(startLoc.y <= wallTL.y)
				{
					endLoc.y = wallTL.y;
				}
				if(startLoc.x >= wallBR.x)
				{
					endLoc.x = wallBR.x;
				}
				if(startLoc.y >= wallBR.y)
				{
					endLoc.y = wallBR.y;
				}
				break;
			}
		}
	}
	return endLoc;
}

void FreezeFrame::onPlayerDeath()
{
	if(player.alive && !l2pCheat)
	{
		player.alive = false;
		spawnParticleCloud(player.getCenter(),graphicsNS::BLUE);
		playerDeathCountdown = TIME_UNTIL_RESET;
		audio->playCue(KILL3_CUE);
	}
}