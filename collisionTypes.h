// Programming 2D Games
// Copyright (c) 2011,2012 by: 
// Charles Kelly
// collisionTypes.h v1.0

#ifndef _COLLISION_TYPES_H      // Prevent multiple definitions if this 
#define _COLLISION_TYPES_H      // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

class CollisionTypes;

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include <cmath>
#include "player.h"
#include "puck.h"
#include "bricks.h"
#include "menu.h"
#include "mainMenu.h"
#include "laser.h"
#include "grunt.h"
#include "zeppelin.h"
#include "patternStep.h"
#include "cheatsMenu.h"
#include "genericMenu.h"

#define maxPatternSteps 4

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
    TextureManager playerTM, puckTM, laserTM, gruntTM, zepTM, healthTM;   // game texture
    Image   playerTexture, puckTexture, laserTexture, gruntTexture, zepTexture, healthTexture;
    VECTOR2 collisionVector;    // collision vector
	Player player;
	Entity health;
	Puck puck;
	Laser laser;
	Grunt grunts[NUMGRUNTS];
	Zeppelin zep;
	int score1;
	int score2;
	bool collision;
	Brick bricks[5];
	// STATE Stuff
	GameStates gameStates;
	Image splashScreen;
	TextureManager splashScreenTM;
	Image gameOver;
	TextureManager gameOverTM;
	float timeInState;
	float timeSinceSpawn;
	int lastGrunt;
	void gameStateUpdate();
	Image tex;
	mainMenu *menu;
	genericMenu *creditsMenu;
	genericMenu *controlsMenu;
	cheatsMenu *cheat;
	TextDX  *waveFont;     
	PatternStep patternSteps[maxPatternSteps];
	int patternStepIndex;
	int timeSinceHit;
	//cheat stuff
	bool invincible;
	bool noDeath;

public:
    // Constructor
    CollisionTypes();

    // Destructor
    virtual ~CollisionTypes();

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
