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

	P1Controls = Controls('W','S','A','D');

	worldSizes = new VECTOR2[GameState::SIZE];
	worldSizes[GameState::TitleScreen] = VECTOR2(GAME_WIDTH,GAME_HEIGHT);
	worldSizes[GameState::Level1] = VECTOR2(4096,4096);
	worldSizes[GameState::Level2] = VECTOR2(2048,2048);
	worldSizes[GameState::Level3] = VECTOR2(2048,2048);

	currentState = TitleScreen;
	
	timeMultiplier = 1;

	P1Controls = Controls('W','S','A','D');
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

	ShowCursor(false);

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
	if(!bulletTrailTex.initialize(graphics,BULLET_TRAIL_IMAGE))
		throw GameError(6,"Failed to init bullet trail tex");
	if(!menuTex.initialize(graphics,MENU_IMAGE))
		throw GameError(7,"Failed to init menu tex");
	if(!menuCursorTex.initialize(graphics,MENU_CURSOR_IMAGE))
		throw GameError(8,"Failed to init menu cursor tex");


	if(!title.initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(9,"Failed to init title");
	title.setCurrentFrame(MENU_TITLE);
	if(!subtitle.initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(10,"Failed to init subtitle");
	subtitle.setCurrentFrame(MENU_SUBTITLE);

	for(int i = 0; i < freezeFrameNS::NUM_MENU_OPTIONS; i++)
	{
		if(!menuItems[i].initialize(graphics,MENU_CELL_WIDTH,MENU_CELL_HEIGHT,1,&menuTex))
		throw GameError(-1*i,"Failed to init menu item");

		menuItems[i].setCurrentFrame(MENU_PLAY+i);
	}

	if(!menuCursor.initialize(graphics,0,0,0,&menuCursorTex))
		throw GameError(11,"Failed to init menu corsor");

	if(!background.initialize(graphics,0,0,0,&backgroundTex))
		throw GameError(7,"Failed to init background image");

	if(!player.initialize(this,P1Controls,0,0,0,&manTex))
		throw GameError(24,"Failed to init player");

	if(!cursor.initialize(this,0,0,0,&cursorTex))
		throw GameError(25,"Failed to init cursor");

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		if(!actors[i].initialize(this,64,64,4,&walkTex))
			throw GameError(-1*i,"FAILED TO MAKE DUDE!");

		actors[i].setCenterX(rand01()*worldSizes[currentState].x);
		actors[i].setCenterY(rand01()*worldSizes[currentState].y);
		actors[i].setColorFilter(graphicsNS::RED);
		actors[i].setFrames(0, 6);   // animation frames
		actors[i].setCurrentFrame(0);     // starting frame
		actors[i].setFrameDelay(0.05f); //0.08 seems appriopriate
	}

	for(int i = 0 ; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].initialize(this,0,0,0,&bulletTrailTex))
			throw GameError(-1*i,"FAILED TO MAKE particle!");
	}

	for(int i = 0 ; i < MAX_PLAYER_BULLETS; i++)
	{
		if(!playerBullets[i].initialize(this,0,0,0,&bulletTex))
			throw GameError(-1*i,"FAILED TO MAKE player bullet!");
	}

	for(int i = 0 ; i < MAX_ENEMY_BULLETS; i++)
	{
		if(!enemyBullets[i].initialize(this,0,0,0,&bulletTex))
			throw GameError(-1*i,"FAILED TO MAKE enemy bullet!");
	}

	player.setColorFilter(COLOR_ARGB(0xFF3E52ED));

	currentState = TitleScreen;
	menuLoad();
	

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
	case Level1:
		level1Update();
	}

}

void FreezeFrame::menuUpdate()
{
	static float counter = 0; 
	static int selectedItem = 0;

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
	selectedItem%= NUM_MENU_OPTIONS;

	if(input->wasKeyPressed(VK_RETURN))
	{
		switch (selectedItem)
		{
		case 0:
			currentState = Level1;
			level1Load();
			break;
		case 3:
			PostQuitMessage(0);
			break;
		default:
			break;
		}
		
	}
}

void FreezeFrame::level1Update()
{
	worldFrameTime = frameTime;
	timeMultiplier = 1;
	player.update(worldFrameTime);
	updateScreen(player.getCenter());


	cursor.update(worldFrameTime);

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		actors[i].update(worldFrameTime);
		
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
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FreezeFrame::ai()
{
	for(int i = 0; i < MAX_ACTORS; i++) {
		actors[i].ai(worldFrameTime, player);
	}
}

//=============================================================================
// Handle collisions
//=============================================================================
void FreezeFrame::collisions()
{
	
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
	case Level1:
		level1Render();
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
	
}

void FreezeFrame::level1Render()
{
	background.draw(screenLoc);

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

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		actors[i].draw(screenLoc,graphicsNS::FILTER);
	}

	player.draw(screenLoc);

	cursor.draw(screenLoc);
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
}

void FreezeFrame::level1Load()
{
	player.setCenter(VECTOR2(1000,1000));

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		actors[i].setCenterX(rand01()*worldSizes[currentState].x);
		actors[i].setCenterY(rand01()*worldSizes[currentState].y);
		actors[i].setColorFilter(graphicsNS::RED);
	}
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


bool FreezeFrame::spawnBullet(VECTOR2 loc, float dir,COLOR_ARGB c, bool playerBullet)
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
			return true;
		}
	}

	return false;
}


bool FreezeFrame::spawnSmokeParticle(VECTOR2 loc, COLOR_ARGB c)
{
	for(int i = 0; i < MAX_PARTICLES; i++)
	{
		if(!particles[i].getActive())
		{
			particles[i].create(loc,c);
			return true;
		}
	}

	return false;
}


