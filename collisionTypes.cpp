// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"
#include <time.h>

bool bossSpawn = 0;

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
	gameStates = intro;

	menu = new mainMenu();
	menu->initialize(graphics, input); //, menuText);

	waveFont = new TextDX();
	if(waveFont->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	if (!playerTM.initialize(graphics,PLAYER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	if (!player.initialize(this, playerTM.getWidth(), playerTM.getHeight(), 0,&playerTM))
		throw(GameError(gameErrorNS::WARNING, "Player not initialized"));
	player.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-2*player.getHeight()));
	player.setCollisionType(entityNS::BOX);
	player.setEdge(COLLISION_BOX_PLAYER);
	player.setCollisionRadius(COLLISION_RADIUS);
	player.setScale(1.0);
	player.setHealth(100);

	if (!laserTM.initialize(graphics,LASER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
	if (!laser.initialize(this,laserNS::WIDTH, laserNS::HEIGHT, laserNS::TEXTURE_COLS, &laserTM))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing background"));
	laser.setScale(1.25);
	laser.setX(10);
	laser.setY(10);
	laser.setVisible(false);

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
		grunts[i].setCurrentFrame(GRUNT_IDLE_START);
		grunts[i].setFrameDelay(.01);
	}


		if (!zepTM.initialize(graphics,ZEP_IMAGE))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
		if (!zep.initialize(this, 0, 0, 0,&zepTM))
			throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
		zep.setPosition(VECTOR2(400, 100));
		zep.setCollision(entityNS::BOX);
		zep.setEdge(COLLISION_BOX_PUCK);
		zep.setX(zep.getPositionX());
		zep.setY(zep.getPositionY());
		zep.setScale(.3);
		zep.setDead(true);

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

	return;
}

//=============================================================================
// Update all game items
//=============================================================================

void CollisionTypes::gameStateUpdate()
{

	timeInState += frameTime;
	if (gameStates==intro && menu->getSelectedItem() == 0)//timeInState > 2.5)
	{
		gameStates = wave1;
		timeInState = 0;
	}
	if (gameStates== wave1 && timeInState > 10)
	{
		gameStates = wave2;
		timeInState = 0;
	}
		if (gameStates== wave2 && timeInState > 60)
	{
		gameStates = end;
		timeInState = 0;
	}
	if (gameStates==end && timeInState > 4)
	{
		PostQuitMessage(0);
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
	if (input->isKeyDown(VK_ESCAPE)) {
		exitGame();
	}
	gameStateUpdate();
	menu->update();
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
	case wave1:
		//spawn grunts
		if(timeSinceSpawn > 3)
		{
			lastGrunt++;
			grunts[lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			timeSinceSpawn = 0;
			if(lastGrunt == NUMGRUNTS)
				lastGrunt = 0;
		}
		if(input->isKeyDown(VK_LEFT))
			player.left();
		if(input->isKeyDown(VK_RIGHT))
			player.right();
		if(input->isKeyDown(VK_UP))
			player.up();
		if(input->isKeyDown(VK_DOWN))
			player.down();
		player.update(frameTime);
		laser.update(frameTime, player, audio);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		zep.update(frameTime);
		break;

	case wave2:
		//spawn grunts
		if(timeSinceSpawn > 3)
		{
			lastGrunt++;
			grunts[lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			timeSinceSpawn = 0;
			if(lastGrunt == NUMGRUNTS)
				lastGrunt = 0;
		}
		if(timeInState > 5)
		{
			if (!bossSpawn)
				zep.spawn();
			bossSpawn = true;
		}
		if(input->isKeyDown(VK_LEFT))
			player.left();
		if(input->isKeyDown(VK_RIGHT))
			player.right();
		if(input->isKeyDown(VK_UP))
			player.up();
		if(input->isKeyDown(VK_DOWN))
			player.down();
		player.update(frameTime);
		laser.update(frameTime, player, audio);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		zep.update(frameTime);
		break;
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	for(int i = 0; i < NUMGRUNTS; i++)
		grunts[i].ai(frameTime, player);
	zep.ai(frameTime, player);
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
	for(int i = 0; i < NUMGRUNTS; i++) {
		if(grunts[i].collidesWith(player) && !grunts[i].getDead()){
			grunts[i].setCollides(true);
			player.setHealth(player.getHealth()-20);
			player.setX(240);
			player.setY(100);
		}
		else
			grunts[i].setCollides(false);

		if(grunts[i].isHitBy(laser) && !grunts[i].getDead()){
			grunts[i].setFrames(GRUNT_EXPLODE_START, GRUNT_EXPLODE_END);
			
			//grunts[i].setDead(true);
		}
		if(grunts[i].getCurrentFrame() == GRUNT_EXPLODE_END)
			grunts[i].setDead(true);
	}

	if(zep.collidesWith(player) && !zep.getDead()){
			zep.setCollides(true);
			player.setHealth(player.getHealth()-20);
			player.setX(240);
			player.setY(100);
		}
		else
			zep.setCollides(false);

		if(zep.isHitBy(laser) && !zep.getDead() && !laser.getDead()){
			zep.setHealth(zep.getHealth() - 10);
			laser.setDead(true);
			laser.setVisible(false);
			if(zep.getHealth() == 0)
				zep.setDead(true);
			//NEED to destroy laser upon hitting 
		}
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
	graphics->spriteBegin(); // begin drawing sprites
	switch (gameStates)
	{
	case intro:
		menu->displayMenu();
		break;
	case wave1:
		if(timeInState < 3)
			waveFont->print("Wave 1",310,100);
		player.draw();
		laser.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		break;
	case wave2:
		if(timeInState < 3)
			waveFont->print("Wave 2",310,100);
		player.draw();
		laser.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		zep.draw();
		break;
		//draw stuff
	case end:
		gameOver.draw();
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

	playerTM.onLostDevice();
	Game::releaseAll();
	return;
}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void CollisionTypes::resetAll()
{

	playerTM.onResetDevice();
	Game::resetAll();
	return;
}
