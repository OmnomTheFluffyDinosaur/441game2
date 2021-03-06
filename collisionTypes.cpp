// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"
#include "laserManager.h"
#include <time.h>
#include "sparkManager.h"
#include "smokeManager.h"
#include <fstream>
#include <string>

LaserManager lm;
SparkManager sm;
SmokeManager smokem;

bool bossSpawn = 0;
bool pickupSpawn = 0;

//=============================================================================
// Constructor
//=============================================================================
CollisionTypes::CollisionTypes()
{
	//nothing here, move on
}

//=============================================================================
// Destructor
//=============================================================================
CollisionTypes::~CollisionTypes()
{
	SAFE_DELETE(waveFont);
	releaseAll();           // call onLostDevice() for every graphics item
}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void CollisionTypes::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError

	timeInState = 0;
	timeSinceSpawn = 0;
	lastGrunt = 0;
	score1 = score2 = 0;
	gameStates = splash;
	invincible = false;
	noDeath = false;
	scoreCount = 0;

	menu = new mainMenu();
	menu->initialize(graphics, input); //, menuText);

	cheat = new cheatsMenu();
	cheat->initialize(graphics, input);

	creditsMenu = new genericMenu("Explosion and laser sounds by dklon from OpenGameArt.org","Background by Downdate from OpenGameArt.org \nMusic by Circlerun from OpenGameArt.org","");
	creditsMenu->initialize(graphics, input);

	controlsMenu = new genericMenu("Press Arrow Keys to Move","Press Space and 'v' to Shoot","");
	controlsMenu->initialize(graphics, input);

	waveFont = new TextDX();
	if(waveFont->initialize(graphics, 40, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));
	scoreFont = new TextDX();
	if(scoreFont->initialize(graphics, 20, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	if (!bgTM.initialize(graphics,BG_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	if (!bgTexture.initialize(graphics,0,0,0,&bgTM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
    bgTexture.setScale(BG_SCALE);

	//if (!mapTM.initialize(graphics,MENU_BACKGROUND))
     //   throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	if (!map.initialize(graphics,0,0,0,&bgTM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
    map.setScale(BG_SCALE);


	if (!splashTM.initialize(graphics,SPLASH))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	if (!splashTexture.initialize(graphics,0,0,0,&splashTM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));

	if (!livesTM.initialize(graphics,PLAYER_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	if (!lives.initialize(graphics,PLAYER_WIDTH,PLAYER_HEIGHT,PLAYER_COLS,&livesTM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
	lives.setScale(.6);

	if (!playerTM.initialize(graphics,PLAYER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	if (!player.initialize(this, PLAYER_WIDTH, PLAYER_HEIGHT, PLAYER_COLS,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "Player not initialized"));
	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-2*player.getHeight()));
	player.setCollisionType(entityNS::BOX);
	player.setEdge(COLLISION_BOX_PLAYER);
	player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(1.0);
	player.setHealth(100);
	player.setLives(3);
	player.setCurrentFrame(PLAYER_IDLE_START);
	player.setFrameDelay(.012);
	timeSinceHit = time(0);
			

	if (!healthTM.initialize(graphics,HEALTH_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	if (!health.initialize(this, HEALTH_WIDTH, HEALTH_HEIGHT, HEALTH_COLS,&healthTM))
		throw(GameError(gameErrorNS::WARNING, "Player not initialized"));
	health.setPosition(VECTOR2(player.getX(), player.getY() + 40));
	health.setCollisionType(entityNS::BOX);
	health.setScale(0.5);
	health.setCurrentFrame(HEALTH_FULL);

	for(int i = 0; i < NUMGRUNTS; i++) {
		if (!gruntTM.initialize(graphics,GRUNT_IMAGE))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
		if (!grunts[i].initialize(this, GRUNT_WIDTH, GRUNT_HEIGHT, GRUNT_COLS,&gruntTM))
			throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		grunts[i].setPosition(VECTOR2(400, 100 + i*40));
		grunts[i].setCollision(entityNS::BOX);
		grunts[i].setEdge(COLLISION_BOX_PUCK);
		grunts[i].setX(grunts[i].getPositionX());
		grunts[i].setY(grunts[i].getPositionY());
		grunts[i].setScale(1);
		grunts[i].setDead(true);
		grunts[i].setInvisible();
		grunts[i].setCurrentFrame(GRUNT_IDLE_START);
		grunts[i].setFrameDelay(.008);
	}

	if (!medTM.initialize(graphics,HEALTH_PICKUP))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
	if (!medPack.initialize(this, 0, 0, 0,&medTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		medPack.setPosition(VECTOR2(400, 100));
		medPack.setCollision(entityNS::BOX);
		medPack.setEdge(COLLISION_BOX_PICKUP);
		medPack.setX(medPack.getPositionX());
		medPack.setY(medPack.getPositionY());
		medPack.setScale(1);
		medPack.setDead(true);
		medPack.setInvisible();

		if (!pointsTM.initialize(graphics,POINT_PICKUP))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
	if (!point.initialize(this, 0, 0, 0,&pointsTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		point.setPosition(VECTOR2(400, 100));
		point.setCollision(entityNS::BOX);
		point.setEdge(COLLISION_BOX_PICKUP);
		point.setX(point.getPositionX());
		point.setY(point.getPositionY());
		point.setScale(1);
		point.setDead(true);
		point.setInvisible();




	if (!zepTM.initialize(graphics,ZEP_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
	if (!zep.initialize(this, ZEP_WIDTH, ZEP_HEIGHT, ZEP_COLS,&zepTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
	zep.setPosition(VECTOR2(400, 100));
	zep.setCollision(entityNS::BOX);
	zep.setEdge(COLLISION_BOX_PUCK);
	zep.setX(zep.getPositionX());
	zep.setY(zep.getPositionY());
	zep.setScale(2);
	zep.setDead(true);
	zep.setInvisible();
	zep.setCurrentFrame(ZEP_IDLE_START);
	zep.setFrameDelay(.01);

	if (!ufoTM.initialize(graphics,UFO_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
	if (!ufo.initialize(this, UFO_WIDTH, UFO_HEIGHT, UFO_COLS,&ufoTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
	ufo.setPosition(VECTOR2(400, 100));
	ufo.setCollision(entityNS::BOX);
	ufo.setEdge(COLLISION_BOX_PUCK);
	ufo.setX(ufo.getPositionX());
	ufo.setY(ufo.getPositionY());
	ufo.setScale(0.25);
	ufo.setDead(true);
	ufo.setInvisible();
	ufo.setCurrentFrame(UFO_START);
	ufo.setFrameDelay(.01);
	ufo.setFrames(UFO_START, UFO_END);
	ufo.setHealth(100);


	//patternsteps
	/*patternStepIndex = 0;
	for (int i = 0; i<maxPatternSteps; i++) {
	patternSteps[i].initialize(&grunts);
	patternSteps[i].setActive();
	}
	patternSteps[0].setAction(RIGHT);
	patternSteps[0].setTimeForStep(3);
	patternSteps[1].setAction(DOWN);
	patternSteps[1].setTimeForStep(2);
	patternSteps[2].setAction(TRACK);
	patternSteps[2].setTimeForStep(4);
	patternSteps[3].setAction(BRTL);
	patternSteps[3].setTimeForStep(2);*/

	audio->playCue("Music");
	bgTexture.setX(0);
	bgTexture.setY(0);

	//background
	/*if (!backTexture1.initialize(graphics, BACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Background texture fail"));
	if (!background1.initialize(graphics, 0,0,0, &backTexture1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Background 1 fail"));
	if (!background2.initialize(graphics, 0,0,0, &backTexture1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Background 2 fail"));

	background2.setColorFilter(graphicsNS::BLACK);
	background2.setX(GAME_WIDTH);

	Vel1.xVel = -60;
	Vel2.yVel = -60;*/

	lm.initialize(graphics, this);
	sm.initialize(graphics);
	smokem.initialize(graphics);
	reloadTime = 0;
	shotReload = 0;
	enemyReload = 0;

	return;
}

//=============================================================================
// Update all game items
//=============================================================================

void createParticleEffect(VECTOR2 pos, VECTOR2 vel, int numParticles){

	lm.setPosition(pos);
	lm.setVelocity(vel);
	lm.setVisibleNParticles(numParticles);

}

void CollisionTypes::createSparkEffect(VECTOR2 pos, VECTOR2 vel, int numParticles){

	sm.setPosition(pos);
	sm.setVelocity(vel);
	sm.setVisibleNSparks(numParticles);

}

void createSmokeEffect(VECTOR2 pos, VECTOR2 vel, int numParticles) {
	smokem.setPosition(pos);
	smokem.setVelocity(vel);
	smokem.setVisibleNParticles(numParticles);
}

void CollisionTypes::gameStateUpdate()
{
	timeInState += frameTime;
	//main menu
	if(gameStates == splash && timeInState > 3.5)
	{
		timeInState = 0;
		gameStates = intro;
	}
	if (gameStates==intro && menu->getSelectedItem() == 0 && timeInState > .05)
	{
		gameStates = wave1;
		timeInState = 0;
	}
	else if (gameStates==intro && menu->getSelectedItem() == 1 && timeInState > .05)
	{
		gameStates = cheats;
		timeInState = 0;
	}
	else if (gameStates==intro && menu->getSelectedItem() == 2 && timeInState > .05)
	{
		gameStates = credits;
		timeInState = 0;
	}
	else if (gameStates==intro && menu->getSelectedItem() == 3 && timeInState > .05)
	{
		gameStates = controls;
		timeInState = 0;
	}

	if  (gameStates==credits && creditsMenu->getSelectedItem() == 2 && timeInState > .05)
	{
		gameStates = intro;
		timeInState = 0;
	}

	if  (gameStates==controls && controlsMenu->getSelectedItem() == 2 && timeInState > .05)
	{
		gameStates = intro;
		timeInState = 0;
	}

	//cheats menu
	if (gameStates==cheats && cheat->getSelectedItem() == 0&& timeInState > .05)
	{
		invincible = !invincible;
		if(invincible)
			cheat->setMenuItem1("Invincible: On");
		else
			cheat->setMenuItem1("Invincible: Off");
		timeInState = 0;
	}
	else if (gameStates==cheats && cheat->getSelectedItem() == 1 && timeInState > .05)
	{
		noDeath = !noDeath;
		if(noDeath)
			cheat->setMenuItem2("Infinite Lives: On");
		else
			cheat->setMenuItem2("Infinite Lives: Off");
		timeInState = 0;
	}
	else if (gameStates==cheats && cheat->getSelectedItem() == 2 && timeInState > .05)
	{
		gameStates = intro;
		timeInState = 0;
	}

	if (gameStates==cheats && cheat->getSelectedItem() == 0 && timeInState > .05)
	{
		//gameStates = wave1;
		timeInState = 0;
	}

	if (gameStates== wave1 && timeInState > 40)	/////////////////////////////////////////////////////////fix state time
	{
		gameStates = wave2;
		timeInState = 0;
	}
	if ((gameStates == wave2) && zep.getCurrentFrame() == ZEP_EXPLODE_END)// timeInState > 45)
	{
		gameStates = end;
		timeInState = 0;
	}
	if ((gameStates == wave1 || gameStates == wave2) && player.getLives() == 0)
	{
		gameStates = gameEnd;
		timeInState = 0;
	}
	if(gameStates == wave1 && player.getHealth() <= 0)
	{
		player.setFrames(PLAYER_EXPLODE_START, PLAYER_EXPLODE_END);
		if(player.getCurrentFrame() == PLAYER_EXPLODE_END) {
			player.setCurrentFrame(PLAYER_IDLE_START);
			player.setFrames(PLAYER_IDLE_START, PLAYER_IDLE_END);
			player.setX(240);
			player.setY(100);
			timeInState = 0;
			score1 = 0;
			for(int i = 0; i < NUMGRUNTS; i++)
			{
				grunts[i].setDead(true);
				grunts[i].setInvisible();
			}
			zep.setDead(true);
			zep.setInvisible();
			player.setHealth(100);
			if(!noDeath)
				player.setLives(player.getLives()-1);
			bossSpawn = 0;
			pickupSpawn = 0;
		}
	}
	if(gameStates == wave2 && player.getHealth() <= 0)
	{
		player.setFrames(PLAYER_EXPLODE_START, PLAYER_EXPLODE_END);
		if(player.getCurrentFrame() == PLAYER_EXPLODE_END) {
			player.setCurrentFrame(PLAYER_IDLE_START);
			player.setFrames(PLAYER_IDLE_START, PLAYER_IDLE_END);
			player.setX(240);
			player.setY(100);
			timeInState = 0;
			score2 = 0;
			for(int i = 0; i < NUMGRUNTS; i++)
			{
				grunts[i].setDead(true);
				grunts[i].setInvisible();
			}
			zep.setDead(true);
			zep.setInvisible();
			player.setHealth(100);
			if(!noDeath)
				player.setLives(player.getLives()-1);
			bossSpawn = 0;
			pickupSpawn = 0;
		}
	}


	if (gameStates==end && timeInState > 10)
	{
		PostQuitMessage(0);
		timeInState = 0;
	}
	if  ((gameStates == end || gameStates == gameEnd) && timeInState > 5)
	{
		gameStates = intro;
		timeInState = 0;
	}
	//gameStates = gamePlay;


	/*if (input->isKeyDown(VK_F1))
	{
	gameStates = readyToPlay;
	}
	if (input->isKeyDown(VK_F2)) {
	gameStates = gamePlay;
	}
	if (input->isKeyDown(VK_F3))
	{
	gameStates= end;
	}*/
}
void CollisionTypes::update()
{
	reloadTime += frameTime;
	shotReload += frameTime;
	enemyReload += frameTime;
	VECTOR2 foo,bar;
	VECTOR2 smokeXY,smokeV;
	if (input->isKeyDown(VK_ESCAPE)) {
		exitGame();
	}
	gameStateUpdate();
	menu->update();
	cheat->update();
	creditsMenu->update();
	controlsMenu->update();
	if (input->isKeyDown(VK_F1)) {
		gameStates = intro;
		health.setCurrentFrame(HEALTH_FULL);
		player.setHealth(100);
		player.setLives(3);
		player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-2*player.getHeight()));
		timeSinceHit=time(0);
		score1=0;
		score2=0;
		scoreCount = 0;
		lm.resetAll();
	}

	/*
	switch (gameStates)
	{
	case intro:

	case readyToPlay:

	case gamePlay:

	}*/
	/*if(input->isKeyDown(VK_LEFT))
	player.left();
	if(input->isKeyDown(VK_RIGHT))
	player.right();
	if(input->isKeyDown(VK_UP))
	player.up();
	if(input->isKeyDown(VK_DOWN))
	player.down();
	player.update(frameTime);
	laser.update(frameTime,player,audio);*/

	timeSinceSpawn += frameTime;
	switch (gameStates)
	{

	case intro:
		//nothing yet
		//case readyToPlay:
		//nothing yet
		break;
	case cheats:
		break;
	case wave1:
		{
		//spawn grunts
		if(timeInState < 37 && timeSinceSpawn > 3)
		{
			for(int i = 0; i < 3; i++){
				grunts[++lastGrunt].spawn();	
				grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
				grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
				if(lastGrunt >= NUMGRUNTS)
					lastGrunt = 1;
			}
			timeSinceSpawn = 0;
		}
		if(!bossSpawn && timeInState > 5) {
		//	ufo.spawn();
		//	ufo.setFrames(UFO_START, UFO_END);
		//	ufo.setDead(false);
		//	ufo.setVisible();
		//	bossSpawn = 1;
		}
		if(timeInState > 15 && !pickupSpawn) {
			if(rand()%2 == 0) {
				medPack.spawn();
				medPack.setDead(false);
			}
			else {
				point.spawn();
				point.setDead(false);
			}
			pickupSpawn = true;
		}
		if(input->isKeyDown(VK_LEFT))
			player.left();
		if(input->isKeyDown(VK_RIGHT))
			player.right();
		if(input->isKeyDown(VK_UP)) {
			player.up();
			if (player.getDegrees() > -9) {
				player.setDegrees(player.getDegrees()-0.5f);
			}
		}
		else if(input->isKeyDown(VK_DOWN)){
			player.down();
			if (player.getDegrees() < 9) {
				player.setDegrees(player.getDegrees()+0.5f);
			}
		}
		else {
			if (player.getDegrees() ==0)
				break;
			else if (player.getDegrees() < 0)
				player.setDegrees(player.getDegrees()+0.5f);
			else if (player.getDegrees() > 0)
				player.setDegrees(player.getDegrees()-0.5f);
		}

		//SHOOTING
		if (reloadTime >= 0.6f) {
			if (input->isKeyDown(VK_SPACE)) {
				foo = VECTOR2(player.getCenterX()+35, player.getCenterY());
				bar = VECTOR2(500,0);
				createParticleEffect(foo, bar, 1);
				reloadTime = 0;
			}
		}
		if (shotReload >= 1.3f) {
			if (input->isKeyDown(0x56)) {
				foo = VECTOR2(player.getCenterX()+35, player.getCenterY());
				bar = VECTOR2(400,0);
				createParticleEffect(foo, bar, 1);
				bar = VECTOR2(400,-200);
				createParticleEffect(foo, bar, 1);
				bar = VECTOR2(400,200);
				createParticleEffect(foo, bar, 1);
				shotReload = 0;
			}
		}
		smokeXY = VECTOR2(player.getCenterX()-30, player.getCenterY()-5);
		smokeV = VECTOR2(-50,-5);
		if (player.getHealth() <=60) {
			createSmokeEffect(smokeXY,smokeV,20);
		}

		lm.update(frameTime);
		sm.update(frameTime);
		smokem.update(frameTime);
		//Math didn't work right
		if(player.getHealth() == 100)
			health.setCurrentFrame(HEALTH_FULL);
		if(player.getHealth() == 80)
			health.setCurrentFrame(HEALTH_80);
		if(player.getHealth() == 60)
			health.setCurrentFrame(HEALTH_60);
		if(player.getHealth() == 40)
			health.setCurrentFrame(HEALTH_40);
		if(player.getHealth() == 20)
			health.setCurrentFrame(HEALTH_20);
		if(player.getHealth() == 0)
			health.setCurrentFrame(HEALTH_00);
		health.setX(player.getX()+7); 
		health.setY(player.getY()+20);
		player.update(frameTime);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		health.update(frameTime);
		medPack.update(frameTime);
		point.update(frameTime);
		ufo.update(frameTime);
		lives.update(frameTime);
		
		//SCROLLING BACKGROUND
		// move space along X
		bgTexture.setX(bgTexture.getX() - (frameTime * player.getVelocity().x*0.3f) - 0.5);
		// move space along Y
		bgTexture.setY(bgTexture.getY() - frameTime * player.getVelocity().y*0.3f);

		// Wrap space image around at edge
		// if left edge of space > screen left edge
		if (bgTexture.getX() > 0)               
			// move space image left by SPACE_WIDTH
			bgTexture.setX(bgTexture.getX() - BG_WIDTH);
		// if space image off screen left
		if (bgTexture.getX() < -BG_WIDTH)
			// move space image right by SPACE_WIDTH
			bgTexture.setX(bgTexture.getX() + BG_WIDTH);
		// if top edge of space > screen top edge
		if (bgTexture.getY() > 0)
			// move space image up by SPACE_HEIGHT
			bgTexture.setY(bgTexture.getY() - BG_HEIGHT);
		// if space image off screen top
		if (bgTexture.getY() < -BG_HEIGHT)
			// move space image down by SPACE_IMAGE
			bgTexture.setY(bgTexture.getY() + BG_HEIGHT);

		VECTOR2 v = D3DXVECTOR2(0,0);
		player.setVelocity(v);
		break;
		}
	case wave2: 
		{
		//spawn grunts
		if(timeSinceSpawn > 1.8 && !bossSpawn)
		{
			for(int i = 0; i < 3; i++){
				grunts[++lastGrunt].spawn();	
				grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
				grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
				if(lastGrunt >= NUMGRUNTS)
					lastGrunt = 1;
			}
			timeSinceSpawn = 0;
		}
		if(timeInState > 15 && !pickupSpawn) {
			if(rand()%2 == 0) {
				medPack.spawn();
				medPack.setDead(false);
			}
			else {
				point.spawn();
				point.setDead(false);
			}
			pickupSpawn = true;
		}
		if(timeInState > 25 && !bossSpawn)
		{
			if (!bossSpawn) {
				zep.spawn();
				zep.setFrames(ZEP_IDLE_START, ZEP_IDLE_END);
			}
			bossSpawn = true;
		}
		if(input->isKeyDown(VK_LEFT))
			player.left();
		if(input->isKeyDown(VK_RIGHT))
			player.right();
		if(input->isKeyDown(VK_UP)) {
			player.up();
			player.setDegrees(-7);
		}
		else if(input->isKeyDown(VK_DOWN)){
			player.down();
			player.setDegrees(7);
		}
		else
			player.setDegrees(0);
		if (reloadTime >= 0.6f) {
			if (input->isKeyDown(VK_SPACE)) {
				foo = VECTOR2(player.getCenterX()+35, player.getCenterY());
				bar = VECTOR2(500,0);
				createParticleEffect(foo, bar, 1);
				reloadTime = 0;
			}
		}
		if (shotReload >= 1.3f) {
			if (input->isKeyDown(0x56)) {
				foo = VECTOR2(player.getCenterX()+35, player.getCenterY());
				bar = VECTOR2(400,0);
				createParticleEffect(foo, bar, 1);
				bar = VECTOR2(400,-200);
				createParticleEffect(foo, bar, 1);
				bar = VECTOR2(400,200);
				createParticleEffect(foo, bar, 1);
				shotReload = 0;
			}
		}

		//
		for(int i = 0; i < NUMGRUNTS; i++)
		{
			if(!grunts[i].getDead() && enemyReload >= 0.38f)
			{
				if(rand()%3 == 0) {
					foo = VECTOR2(grunts[i].getCenterX()-35, grunts[i].getCenterY());
					bar = VECTOR2(-500,0);
					createParticleEffect(foo, bar, 1);
					enemyReload = 0;
				}
			}
		}

		//
		smokeXY = VECTOR2(player.getCenterX()-30, player.getCenterY()-5);
		smokeV = VECTOR2(-50,-5);
		if (player.getHealth() <=60) {
			createSmokeEffect(smokeXY,smokeV,20);
		}
		
		lm.update(frameTime);
		sm.update(frameTime);
		smokem.update(frameTime);
		if(player.getHealth() == 100)
			health.setCurrentFrame(HEALTH_FULL);
		if(player.getHealth() == 80)
			health.setCurrentFrame(HEALTH_80);
		if(player.getHealth() == 60)
			health.setCurrentFrame(HEALTH_60);
		if(player.getHealth() == 40)
			health.setCurrentFrame(HEALTH_40);
		if(player.getHealth() == 20)
			health.setCurrentFrame(HEALTH_20);
		if(player.getHealth() == 0)
			health.setCurrentFrame(HEALTH_00);
		player.update(frameTime);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		zep.update(frameTime);
		health.setX(player.getX()+7); 
		health.setY(player.getY()+20);
		health.update(frameTime);
		medPack.update(frameTime);
		point.update(frameTime);
		lives.update(frameTime);

		// move space along X
		bgTexture.setX(bgTexture.getX() - frameTime * player.getVelocity().x*0.1f);
		// move space along Y
		bgTexture.setY(bgTexture.getY() - frameTime * player.getVelocity().y*0.1f);

		// Wrap space image around at edge
		// if left edge of space > screen left edge
		if (bgTexture.getX() > 0)               
			// move space image left by SPACE_WIDTH
			bgTexture.setX(bgTexture.getX() - BG_WIDTH);
		// if space image off screen left
		if (bgTexture.getX() < -BG_WIDTH)
			// move space image right by SPACE_WIDTH
			bgTexture.setX(bgTexture.getX() + BG_WIDTH);
		// if top edge of space > screen top edge
		if (bgTexture.getY() > 0)
			// move space image up by SPACE_HEIGHT
			bgTexture.setY(bgTexture.getY() - BG_HEIGHT);
		// if space image off screen top
		if (bgTexture.getY() < -BG_HEIGHT)
			// move space image down by SPACE_IMAGE
			bgTexture.setY(bgTexture.getY() + BG_HEIGHT);

		VECTOR2 vb = D3DXVECTOR2(0,0);
		player.setVelocity(vb);

		break;
			}
	}
	
	/*float x = background1.getCenterX() + Vel1.xVel * frameTime;
	background1.setX(x);
	x = background2.getCenterX() + Vel1.xVel * frameTime;
	background2.setX(x);

	if((background1.getCenterX() + 50) < 0)
		background1.setX(GAME_WIDTH);
	if((background2.getCenterX() + 50) < 0)
		background2.setX(GAME_WIDTH);*/
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	for(int i = 0; i < NUMGRUNTS; i++) {
		switch(gameStates) {
			case wave1:
				grunts[i].ai1(frameTime, player);
			/*	for(int j = 0; j < NUMGRUNTS; j++)
					{
						if(j != i)
							grunts[i].evade(grunts[j]);
					}*/
				break;
			case wave2:
				grunts[i].ai2();
				/*for(int j = 0; j < NUMGRUNTS; j++)
					{
						if(j != i)
							grunts[i].evade(grunts[j]);
					}*/
				break;
		}
	}
	ufo.ai(frameTime);
	zep.ai(frameTime, player);
	medPack.ai();
	point.ai();
	/*if (patternStepIndex == maxPatternSteps)
	return;
	if (patternSteps[patternStepIndex].isFinished())
	patternStepIndex++;
	patternSteps[patternStepIndex].update(frameTime);*/
}

//=============================================================================
// Handle collisions
//=============================================================================
void CollisionTypes::collisions()
{
	VECTOR2 foo;
	VECTOR2 bar;
	for(int i = 0; i < NUMGRUNTS; i++) {
		if(grunts[i].collidesWith(player) && !grunts[i].getDead()){
			grunts[i].setCollides(true);
			if(!invincible) {
				player.setHealth(player.getHealth()-20);
			}
			grunts[i].setFrames(GRUNT_EXPLODE_START, GRUNT_EXPLODE_END);
			grunts[i].setDead(true);
			audio->playCue(BOOM9);
			timeSinceHit = time(0);
			//player.setColorFilter(D3DCOLOR_RGBA(0,255,0,0));
			//player.setX(240);
			//player.setY(100);
		}
		else
			grunts[i].setCollides(false);


		/*if(grunts[i].getVelocity() == D3DXVECTOR2(0,0) && timeSinceSpawn > 1.5) 
		{
			grunts[i].setDead(true);
			grunts[i].setInvisible();
		}*/

		if (!grunts[i].getDead()) {
			if(lm.collidesWith(grunts[i])  ){
				grunts[i].setFrames(GRUNT_EXPLODE_START, GRUNT_EXPLODE_END);
				grunts[i].setDead(true);
				foo = VECTOR2(grunts[i].getCenterX()-grunts[i].getWidth()/2, grunts[i].getCenterY());
				bar = VECTOR2(1,1);
				createSparkEffect(foo, bar, 100);
				audio->playCue(BOOM9);
				switch(gameStates) {
				case wave1:
					score1 += grunts[i].getScore();
					break;
				case wave2:
					score2+= grunts[i].getScore()*1.5;
					break;
				}
			}
		}
		if(grunts[i].getCurrentFrame() == GRUNT_EXPLODE_END)
			grunts[i].setInvisible();
		if (grunts[i].getCurrentFrame() == GRUNT_EXPLODE_START) {
			grunts[i].setScale(2.5f);
			grunts[i].setPositionY(grunts[i].getPositionY()-18);
			grunts[i].setPositionX(grunts[i].getPositionX()-10);
		}
		if (grunts[i].getCurrentFrame() == GRUNT_IDLE_START)
			grunts[i].setScale(1.0f);
	}

	if(medPack.collidesWith(player) && !medPack.getDead()){
		medPack.setCollides(true);
		player.setHealth(player.getHealth()+20);
		medPack.setDead(true);
		medPack.setInvisible();
	}
	if(point.collidesWith(player) && !point.getDead()){
		point.setCollides(true);
		switch(gameStates) {
			case wave1:
				score1 += 500;
				break;
			case wave2:
				score2+= 500;
				break;
		}
		point.setDead(true);
		point.setInvisible();
	}
	else
		point.setCollides(false);

		if(point.collidesWith(player) && !point.getDead()){
		point.setCollides(true);
		player.setHealth(player.getHealth()+20);
		point.setDead(true);
		point.setInvisible();
	}
	else
		point.setCollides(false);
	
		if(lm.collidesWith(player))
		{
			player.setHealth(player.getHealth()-20);

		}
		//add this guy in
if(ufo.collidesWith(player) && !ufo.getDead()){
			ufo.setCollides(true);
			player.setHealth(player.getHealth()-20);
			player.setX(240);
			player.setY(100);
		}
		else
			ufo.setCollides(false);

		if (!ufo.getDead()) {
			if(lm.collidesWith(ufo)) {
				ufo.setHealth(ufo.getHealth() - 20);
				if(ufo.getHealth() <= 0) {
					ufo.setDead(true);
					ufo.setInvisible();
					audio->playCue(BOOM4);
				
					switch(gameStates) {
					case wave1:					//Not necessary at moment, since the ufo only spawns in wave 1
						score1 += ufo.getScore();
						break;
				//	case wave2:
				//		score2+= ufo.getScore();
				//		break;
					}
			
				}
			}
		}


	if(zep.collidesWith(player) && !zep.getDead()){
			zep.setCollides(true);
			player.setHealth(player.getHealth()-20);
			player.setX(240);
			player.setY(100);
		}
		else
			zep.setCollides(false);

		if (!zep.getDead()) {
			if(lm.collidesWith(zep)) {
				zep.setHealth(zep.getHealth() - 4);
				if(zep.getHealth() <= 0) {
					zep.setDead(true);
					audio->playCue(BOOM4);
					zep.setFrames(ZEP_EXPLODE_START, ZEP_EXPLODE_END);
				
					switch(gameStates) {
				//	case wave1:					//Not necessary at moment, since the zep only spawns in wave 2
				//		score1 += zep.getScore();
				//		break;
					case wave2:
						score2+= zep.getScore();
						break;
					}
			
				}
			}
		}

		if(zep.getCurrentFrame() == ZEP_EXPLODE_END)
			zep.setInvisible();
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
	if (scoreCount < score1+score2)
			scoreCount++;
	float x = bgTexture.getX();
    float y = bgTexture.getY();
	graphics->spriteBegin(); // begin drawing sprites
	std::ifstream inHiScore("highscore.txt");
	std::string str; 
	int hiScore;
	switch (gameStates)
	{
	case splash:
		splashTexture.draw();
		break;
	case intro:
		map.draw();
		menu->displayMenu();
		break;
	case cheats:
		map.draw();
		cheat->displayMenu();
		break;
	case credits:
		map.draw();
		creditsMenu->displayMenu();
		break;
	case controls:
		map.draw();
		controlsMenu->displayMenu();
		break;
	case wave1:
	//	background1.draw();
	//	background2.draw();
		// Wrap space image around at edge
		// The Scrolling Bitmap example uses if statements so the space image is
		// only drawn when necessary. This code always draws the space image even
		// when it may be off screen.
		bgTexture.draw();
		bgTexture.setX(bgTexture.getX() + BG_WIDTH);
		bgTexture.draw();
		bgTexture.setY(bgTexture.getY() + BG_HEIGHT);
		bgTexture.draw();
		bgTexture.setX(x);
		bgTexture.draw();
		bgTexture.setY(y);
		health.draw();
		for(int i = 0; i < 3; i++)
		{
			lives.setX(70 + i*40);
			lives.setY(20);
			if(player.getLives() > i)
				lives.draw();
		}
		scoreFont->print("Lives: \nScore: " + std::to_string(scoreCount), 20, 20);
		if(timeInState < 3)
			waveFont->print("Wave 1",300,100);
		if(difftime(time(0), timeSinceHit) < .2)
			player.draw(D3DCOLOR_RGBA(255,0,0,255));
		else 
			player.draw();
		medPack.draw();
		point.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		ufo.draw();
		lm.draw();
		sm.draw();
		smokem.draw();
		break;
	case wave2:

		// Wrap space image around at edge
		// The Scrolling Bitmap example uses if statements so the space image is
		// only drawn when necessary. This code always draws the space image even
		// when it may be off screen.
		bgTexture.draw();
		bgTexture.setX(bgTexture.getX() + BG_WIDTH);
		bgTexture.draw();
		bgTexture.setY(bgTexture.getY() + BG_HEIGHT);
		bgTexture.draw();
		bgTexture.setX(x);
		bgTexture.draw();
		bgTexture.setY(y);
		health.draw();
		for(int i = 0; i < 3; i++)
		{
			lives.setX(70 + i*40);
			lives.setY(20);
			if(player.getLives() > i)
				lives.draw();
		}
		scoreFont->print("Lives: \nScore: " + std::to_string(scoreCount), 20, 20);
		if(timeInState < 3)
			waveFont->print("Wave 2",300,100);
		if(difftime(time(0), timeSinceHit) < .2)
			player.draw(D3DCOLOR_RGBA(255,0,0,255));
		else 
			player.draw();
		medPack.draw();
		point.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		lm.draw();
		sm.draw();
		smokem.draw();
		if(zep.getVisible())
			zep.draw();
		break;
		//draw stuff
	case end:
		map.draw();
		std::getline(inHiScore, str);
		hiScore = atoi(str.c_str());
		if ((score1 + score2) > atoi(str.c_str())) {
			hiScore = (score1 + score2);
			inHiScore.close();
			std::ofstream outHiScore("highscore.txt", std::ios::out | std::ios::trunc);
			outHiScore << (score1 + score2);
			outHiScore.close();
		}

		waveFont->print("Congratulations \n\nYour score is: " + std::to_string(score1 + score2) + "\n\n\n High score: " + std::to_string(hiScore),190,100);
		break;
	
	case gameEnd:
		map.draw();
		waveFont->print("Game Over \n\nYour score is: " + std::to_string(score1 + score2),190,100);
		break;
	}
	/*switch (gameStates)
	{
	case intro:
	//nothing
	case readyToPlay:
	//nothing
	case gamePlay:
	player.draw();
	case end:
	//nothing
	}*/
	graphics->spriteEnd();                  // end drawing sprites
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void CollisionTypes::releaseAll()
{
	backTexture1.onLostDevice();
	zepTM.onLostDevice();
	healthTM.onLostDevice();
	gruntTM.onLostDevice();
	playerTM.onLostDevice();
	bgTM.onLostDevice();
	laserTM.onLostDevice();
	ufoTM.onLostDevice();
	splashTM.onLostDevice();
	pointsTM.onLostDevice();
	medTM.onLostDevice();
	livesTM.onLostDevice();
	mapTM.onLostDevice();
	splashScreenTM.onLostDevice();
	gameOverTM.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CollisionTypes::resetAll()
{
	backTexture1.onResetDevice();
	zepTM.onResetDevice();
	healthTM.onResetDevice();
	gruntTM.onResetDevice();
	playerTM.onResetDevice();
	bgTM.onResetDevice();
	laserTM.onResetDevice();
	ufoTM.onResetDevice();
	splashTM.onResetDevice();
	pointsTM.onResetDevice();
	medTM.onResetDevice();
	livesTM.onResetDevice();
	mapTM.onResetDevice();
	splashScreenTM.onResetDevice();
	gameOverTM.onResetDevice();
	Game::resetAll();
	return;
}
