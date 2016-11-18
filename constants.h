// Programming 2D Games
// Copyright (c) 2011 by: 
// Charles Kelly
// Chapter 6 constants.h v1.1

#ifndef _CONSTANTS_H            // Prevent multiple definitions if this 
#define _CONSTANTS_H            // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//=============================================================================
// Function templates for safely dealing with pointer referenced items.
// The functions defined by these templates may be called using a normal
// function call syntax. The compiler will create a function that replaces T
// with the type of the calling parameter.
//=============================================================================
// Safely release pointer referenced item
template <typename T>
inline void safeRelease(T& ptr)
{
    if (ptr)
    { 
        ptr->Release(); 
        ptr = NULL;
    }
}
#define SAFE_RELEASE safeRelease            // for backward compatiblility

// Safely delete pointer referenced item
template <typename T>
inline void safeDelete(T& ptr)
{
    if (ptr)
    { 
        delete ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE safeDelete              // for backward compatiblility

// Safely delete pointer referenced array
template <typename T>
inline void safeDeleteArray(T& ptr)
{
    if (ptr)
    { 
        delete[] ptr; 
        ptr = NULL;
    }
}
#define SAFE_DELETE_ARRAY safeDeleteArray   // for backward compatiblility

// Safely call onLostDevice
template <typename T>
inline void safeOnLostDevice(T& ptr)
{
    if (ptr)
        ptr->onLostDevice(); 
}
#define SAFE_ON_LOST_DEVICE safeOnLostDevice    // for backward compatiblility

// Safely call onResetDevice
template <typename T>
inline void safeOnResetDevice(T& ptr)
{
    if (ptr)
        ptr->onResetDevice(); 
}
#define SAFE_ON_RESET_DEVICE safeOnResetDevice  // for backward compatiblility

//=============================================================================
//                  Constants
//=============================================================================

// window
const char CLASS_NAME[] = "Collisions";
const char GAME_TITLE[] = "UniPong";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  640;               // width of game in pixels
const UINT GAME_HEIGHT = 480;               // height of game in pixels

// game
const double PI = 3.14159265;
const float FRAME_RATE = 200.0f;                // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const RECT  COLLISION_RECTANGLE = {-30,-16,30,16}; 
const RECT  COLLISION_BOX_PLAYER = {-45,-10,45,10};
const RECT COLLISION_BOX_PUCK = {-32, -32, 32, 32};
const float   COLLISION_RADIUS = 29;
const float BG_SCALE = 1.5f;                 // scale factor of space image
const int   BG_WIDTH = (int)(512 * BG_SCALE);  // width of scaled space image
const int   BG_HEIGHT = (int)(512 * BG_SCALE); // height of scaled space image

// graphic images
const char PUCK_IMAGE[] = "pictures\\PS4_new.png";  // game textures
const char PLAYER_IMAGE[] = "pictures\\biplane.png";      // menu texture
const char LASER_IMAGE[] = "pictures\\laser.png";
const char GRUNT_IMAGE[] = "pictures\\grunt.png";
const char ZEP_IMAGE[] = "pictures\\Zeppelin.png";
const char HEALTH_IMAGE[] = "pictures\\health.png";
const char BACKGROUND_IMAGE[] = "pictures\\sky2.jpg";
const char BG_IMAGE[] = "pictures\\sky3.jpg";
const char SPLASH[] = "pictures\\Splash.png";

const int  GRUNT_COLS = 8;
const int  GRUNT_WIDTH = 64;
const int  GRUNT_HEIGHT = 32;
const int  GRUNT_IDLE_START = 0;
const int  GRUNT_IDLE_END = 0;
const int  GRUNT_EXPLODE_START = 8;
const int  GRUNT_EXPLODE_END = 71;

const int  ZEP_COLS = 8;
const int  ZEP_WIDTH = 128;
const int  ZEP_HEIGHT = 64;
const int  ZEP_IDLE_START = 0;
const int  ZEP_IDLE_END = 0;
const int  ZEP_EXPLODE_START = 8;
const int  ZEP_EXPLODE_END = 71;

const int HEALTH_COLS = 3;
const int HEALTH_WIDTH = 64;
const int HEALTH_HEIGHT = 64;
const int HEALTH_FULL = 0;
const int HEALTH_80 = 1;
const int HEALTH_60 = 2;
const int HEALTH_40 = 3;
const int HEALTH_20 = 4;
const int HEALTH_00 = 5;

// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;       // escape key
const UCHAR ALT_KEY      = VK_MENU;         // Alt key
const UCHAR PLAYER_LEFT    = VK_LEFT;     // left arrow
const UCHAR PLAYER_RIGHT   = VK_RIGHT;    // right arrow
const UCHAR ENTER_KEY    = VK_RETURN; 

//

// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[]  = "audio\\Win\\WavesExample1.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";

// audio cues
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[]   = "hit";
const char BOOM9[] = "Boom9";
const char BOOM4[] = "Boom4";
const char LASER1[] = "Laser1";
const char MUSIC[] = "Music";

enum GameStates {splash, intro, cheats, credits, controls, readyToPlay, wave1, wave2, end, gameEnd};
const char SPLASHSCREEN_IMAGE[] = "pictures\\MyGame.png"; 
const char GAMEOVER_IMAGE[] = "pictures\\MyGameOver.jpg"; 
const char TEX_IMAGE[] = "pictures\\texture1.bmp";

enum PATTERN_STEP_ACTION {NONE, UP, DOWN, LEFT, RIGHT, TRACK, EVADE, BRTL};

const int NUMGRUNTS = 24;

#endif
