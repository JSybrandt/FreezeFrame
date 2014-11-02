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
	P1Controls = Controls('W','S','A','D');

	worldSizes = new VECTOR2[GameState::SIZE];
	worldSizes[GameState::Level1] = VECTOR2(4096,4096);
	worldSizes[GameState::Level2] = VECTOR2(2048,2048);
	worldSizes[GameState::Level3] = VECTOR2(2048,2048);

	currentState = Level1;
	

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


	if(!background.initialize(graphics,0,0,0,&backgroundTex))
		throw GameError(5,"Failed to init background image");

	if(!player.initialize(this,P1Controls,0,0,0,&manTex))
		throw GameError(24,"Failed to init player");
	player.setColorFilter(COLOR_ARGB(0xFF3E52ED));

	player.setCenter(VECTOR2(1000,1000));

	currentState = GameState::Level1;

	srand(time(0));

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		//if(!actors[i].initialize(this,64,64,4,&walkTex))
		if(!actors[i].initialize(this,0,0,0,&manTex))
			throw GameError(-1*i,"FAILED TO MAKE DUDE!");
		actors[i].setCenterX(rand01()*worldSizes[currentState].x);
		actors[i].setCenterY(rand01()*worldSizes[currentState].y);
		actors[i].setColorFilter(graphicsNS::RED);
		//actors[i].setFrames(0, 3);   // animation frames
		//actors[i].setCurrentFrame(1);     // starting frame
		//actors[i].setFrameDelay(0.08f);
	}




	return;
}

//=============================================================================
// Update all game items
//=============================================================================
void FreezeFrame::update()
{
	worldFrameTime = frameTime;
	player.update(worldFrameTime);
	updateScreen(player.getCenter());

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		actors[i].update(worldFrameTime);
	}

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void FreezeFrame::ai()
{}

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

	background.draw(screenLoc);

	for(int i = 0; i < MAX_ACTORS; i++)
	{
		actors[i].draw(screenLoc,graphicsNS::FILTER);
	}

	player.draw(screenLoc);

	graphics->spriteEnd();                  // end drawing sprites


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


void FreezeFrame::updateScreen(VECTOR2 center)
{
	//TODO: nicer screen centering
	screenLoc = VECTOR2(center.x - GAME_WIDTH/2,center.y-GAME_HEIGHT/2);

	if(screenLoc.x<0)screenLoc.x=0;
	if(screenLoc.y<0)screenLoc.y=0;
	if(screenLoc.x+GAME_WIDTH>worldSizes[currentState].x)screenLoc.x=worldSizes[currentState].x-GAME_WIDTH;
	if(screenLoc.y+GAME_HEIGHT>worldSizes[currentState].y)screenLoc.y=worldSizes[currentState].y-GAME_HEIGHT;

}