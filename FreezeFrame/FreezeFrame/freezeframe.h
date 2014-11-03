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
#include "Cursor.h"
#include "Bullet.h"
#include "Particle.h"
#include "Exit.h"
#include "Turret.h"


namespace freezeFrameNS
{
	const int MAX_GUARDS = 100;
	const int MAX_TURRETS = 10;
	const int MAX_PLAYER_BULLETS = 100;
	const int MAX_ENEMY_BULLETS = 1000;
	const int MAX_PARTICLES = 10000;
	const int MAX_SCENERY = 1000;

	const int NUM_MENU_OPTIONS = 4;
	const float MENU_ITEM_SPEED = 300;
	const float MENU_ITEM_DELAY = 0.25;

	const int NUM_PARTICLES_IN_SMOKE_EFFECT = 300;
	const int NUM_PARTICLES_IN_CONE_EFFECT = 150;
}

using namespace freezeFrameNS;
using namespace utilityNS;

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
	TextureManager baseTex;
	TextureManager manTex;
	TextureManager walkTex;
	TextureManager feetTex;
	TextureManager turretTex; 
	TextureManager bulletTex;   
	TextureManager cursorTex;
	TextureManager particleTex;
	TextureManager lineTex;
	TextureManager exitTex;

	Exit exit;

	TextureManager menuCursorTex;
	TextureManager menuTex;

	Image title;
	Image subtitle;
	Image menuItems[NUM_MENU_OPTIONS];
	Image menuCursor;

	Cursor cursor;
	Image   background;         // backdrop image

	Guard guards[MAX_GUARDS];
	Turret turrets[MAX_TURRETS];
	Actor  bases[MAX_TURRETS];
	Bullet playerBullets[MAX_PLAYER_BULLETS];
	Bullet enemyBullets[MAX_ENEMY_BULLETS];
	Particle particles[MAX_PARTICLES];

	Controls P1Controls;
	Player player;

	float worldFrameTime;
	
	

	VECTOR2 screenLoc;

	VECTOR2 * worldSizes; //array of sizes per level

	GameState currentState;

public:
	float timeMultiplier;
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

	VECTOR2 getPlayerLoc(){
		return player.getCenter();
	}

	bool spawnBullet(VECTOR2 loc, float dir,COLOR_ARGB c, bool playerBullet);
	bool spawnParticle(VECTOR2 loc,VECTOR2 vel, COLOR_ARGB c);
	bool spawnTurret(VECTOR2 loc, float dir);
	
	void spawnParticleCloud(VECTOR2 loc, COLOR_ARGB c);
	void spawnParticleCone(VECTOR2 loc, float dir, COLOR_ARGB c);

	void menuLoad();
	void menuUpdate();
	void menuRender();

	void level1Load();
	void level2Load();

	void levelsUpdate();
	void levelsRender();


	void deactivateAll();


};

#endif
