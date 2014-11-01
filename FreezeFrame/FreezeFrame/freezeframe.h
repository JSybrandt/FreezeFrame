// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 4 spacewar.h v1.0

#ifndef _REDSPACE_H             // prevent multiple definitions if this 
#define _REDSPACE_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "Controls.h"
#include <string>
using std::string;
#include <ctime>


//=============================================================================
// Create game class
//=============================================================================
class FreezeFrame : public Game
{
private:
    // variables
	TextureManager backgroundTex;   
	Image   background;         // backdrop image
	Controls P1Controls;

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
};

#endif
