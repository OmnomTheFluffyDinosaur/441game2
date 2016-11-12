// Programming 2D Games
// Copyright (c) 2011, 2012 by: 
// Charles Kelly
// Collision types demo
// Press '1', '2' or '3' to select collision type for ship.

#include "collisionTypes.h"

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
	gameStates = intro;

	menu = new mainMenu();
	menu->initialize(graphics, input); //, menuText);

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

	if (!gruntTM.initialize(graphics,GRUNT_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing puck textures"));
	if (!grunts.initialize(this, 0, 0, 0,&gruntTM))
		throw(GameError(gameErrorNS::WARNING, "Brick not initialized"));
	grunts.setPosition(VECTOR2(400, 100));
	grunts.setCollision(entityNS::BOX);
	grunts.setEdge(COLLISION_BOX_PUCK);
	grunts.setX(grunts.getPositionX());
	grunts.setY(grunts.getPositionY());
	grunts.setScale(1);

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
		gameStates = gamePlay;
		timeInState = 0;
	}
	if (gameStates==gamePlay && input->isKeyDown(VK_F1))
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


	switch (gameStates)
	{
	case intro:
		//nothing yet
		//case readyToPlay:
		//nothing yet
		break;
	case gamePlay:
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
		grunts.update(frameTime);
		break;
	}
}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void CollisionTypes::ai()
{
	grunts.ai(frameTime, player);
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
	if(grunts.collidesWith(player) && !grunts.getDead()){
		grunts.setCollides(true);
		player.setHealth(player.getHealth()-20);
		player.setX(240);
		player.setY(100);
	}
	else
		grunts.setCollides(false);

	if(grunts.isHitBy(laser) && !grunts.getDead()){
		grunts.setDead(true);
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
	case gamePlay:
		player.draw();
		laser.draw();
		grunts.draw();
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
