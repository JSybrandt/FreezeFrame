// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _REDSPACE_H             // prevent multiple definitions if this 
#define _REDSPACE_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include <string>
using std::string;
#include <ctime>


#include "Controls.h"
#include "Actor.h"
#include "Player.h"
#include "Guard.h"
//=============================================================================
// Create game class
//=============================================================================
class FreezeFrame : public Game
{
private:

	enum GameState{
	TitleScreen,
	Level1,
	Level2,
	Level3,
	RestartScreen,
	SIZE //THIS MUST BE THE LAST ELEMENT
	};

    // variables
	TextureManager backgroundTex;   
	TextureManager manTex;
	TextureManager turretTex; 
	TextureManager bulletTex;   
	Image   background;         // backdrop image
	Guard actors[MAX_ACTORS];
	Controls P1Controls;
	Player player;

	float worldFrameTime;

	VECTOR2 screenLoc;

	VECTOR2 * worldSizes; //array of sizes per level

	GameState currentState;

	float rand01(){return ((rand()%100)/100.0);}

public:
    // Constructor
    FreezeFrame();

    // Destructor
    virtual ~FreezeFrame();

    // Initialize the game
    void initialize(HWND hwnd);
    void update();      // must override pure virtual from Game
    void ai();          // "
    void collisions();  // "
    void render();      // "
    void releaseAll();
    void resetAll();


	//places the screen so the selected location is in the middle area (might not center)
	void updateScreen(VECTOR2 center); 

	VECTOR2 getMouseInWorld(){
		VECTOR2 mouse(input->getMouseX(),input->getMouseY());
		return screenLoc + mouse;
	}

};

#endif
