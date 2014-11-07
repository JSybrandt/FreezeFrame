#pragma once

// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 5 constants.h v1.0
#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)

//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// graphic images
const char BACKGROUND_IMAGE[] = "pictures\\background.png";
const char WALK_IMAGE[] = "pictures\\walkingAni.png";
const char FEET_IMAGE[] = "pictures\\feetAni.png";
const char MAN_IMAGE[] = "pictures\\shootingAni.png";
const char TURRET_IMAGE[] = "pictures\\turretAni.png";
const char BASE_IMAGE[] = "pictures\\turretBase.png";
const char BULLET_IMAGE[] = "pictures\\bullet.png";
const char CURSOR_IMAGE[] = "pictures\\cursor.png";
const char PARTICLE_IMAGE[] = "pictures\\particle.png";
const char LINE_IMAGE[] = "pictures\\line2.png";
const char EXIT_IMAGE[] = "pictures\\exit.png";
const char WALL_IMAGE[] = "pictures\\wall.png";
const char CYLINDER_IMAGE[] = "pictures\\cylinder.png";
const char MINE_IMAGE[] = "pictures\\mineAni.png";

const char MENU_IMAGE[] = "pictures\\menu.png";
const char MENU_CURSOR_IMAGE[] = "pictures\\menuCursor.png";

//menu cells
const int MENU_CELL_WIDTH = 442;
const int MENU_CELL_HEIGHT = 78;
const int MENU_TITLE = 0;
const int MENU_SUBTITLE = 1;
const int MENU_PLAY = 2;
const int MENU_FEELING_LUCKY = 3;
const int MENU_OPTIONS = 4;
const int MENU_QUIT = 5;



//Sound Locations
const char WAVE_BANK[] = "audio\\Win\\Wave Bank.xwb";
const char SOUND_BANK[] = "audio\\win\\Sound Bank.xsb";
const char XGS_FILE[]   = "audio\\Win\\AudioProject.xgs";

//Cue Names
const char SC_LAUNCH[] = "launch"; //MUST be exactly the same as cue name
const char LASER_CUE[] = "laser";

// window
const char CLASS_NAME[] = "Freeze Frame";
const char GAME_TITLE[] = "Freeze Frame";
const bool FULLSCREEN = false;              // windowed or fullscreen DO NOT RUN FULLSCREEN IN DEBUG MODE
const UINT GAME_WIDTH =  1280;               // width of screen in pixels
const UINT GAME_HEIGHT = 800;               // height of screen in pixels

//Walking Cells
const int  WALK_COL = 4;
const int  WALK_WIDTH = 64;
const int  WALK_HEIGHT = 64;
const float WALK_DELAY = 0.05f;

//Feet Cells
const int  FEET_COL = 3;
const int  FEET_WIDTH = 64;
const int  FEET_HEIGHT = 64;
const float FEET_DELAY = 0.05f;

//Player Cells
const int  PL_COL = 3;
const int  PL_WIDTH = 64;
const int  PL_HEIGHT = 64;
const float PL_DELAY = 0.05f;

//Turret Cells
const int  TURRET_COL = 3;
const int  TURRET_WIDTH = 64;
const int  TURRET_HEIGHT = 64;
const float TURRET_DELAY = 0.05f;
const int  BASE_WIDTH = 128;
const int  BASE_HEIGHT = 128;

//Mine Cells
const int  MINE_COL = 2;
const int  MINE_WIDTH = 32;
const int  MINE_HEIGHT = 32;
const float MINE_DELAY = 0.10f;
 
// game
const double PI = 3.14159265;
const float FRAME_RATE  = 100.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const enum COLLISION_TYPE {NONE, CIRCLE, BOX, ROTATED_BOX};
const enum WEAPON {ROCK, MISSILE, PLANET};

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key




#endif
