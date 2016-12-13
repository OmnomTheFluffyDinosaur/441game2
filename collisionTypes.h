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
#include "med.h"
#include "point.h"
#include "UFO.h"

#define maxPatternSteps 4

//=============================================================================
// This class is the core of the game
//=============================================================================
class CollisionTypes : public Game
{
private:
    // game items
    TextureManager playerTM, puckTM, laserTM, gruntTM, zepTM, ufoTM, healthTM, bgTM, splashTM, pointsTM, medTM, livesTM;   // game texture
    Image   playerTexture, puckTexture, gruntTexture, zepTexture, ufoTexture, healthTexture, bgTexture, splashTexture, med, points, lives;
    VECTOR2 collisionVector;    // collision vector
	Player player;
	Ufo ufo;
	Entity health;
	Puck puck;
	Grunt grunts[NUMGRUNTS];
	medPack medPack;
	point point;
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
	TextDX  *waveFont, *scoreFont;     
	PatternStep patternSteps[maxPatternSteps];
	int patternStepIndex;
	int timeSinceHit;
	//cheat stuff
	bool invincible;
	bool noDeath;
	//background
	float background1XVel;
	float background2XVel;
	TextureManager backTexture1;
	Image background1;
	Image background2;
	int scoreCount;
	float reloadTime;
	float shotReload;
	float enemyReload;

	struct position{
		float xPos;
		float yPos;
		position() {xPos = 0; yPos = 0;}
	} Pos1, Pos2;

	struct velocity{
		float xVel;
		float yVel;
		velocity() {xVel = 0; yVel = 0;}
	} Vel1, Vel2;

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
	void createSparkEffect(VECTOR2 pos, VECTOR2 vel, int numParticles);

};

#endif
