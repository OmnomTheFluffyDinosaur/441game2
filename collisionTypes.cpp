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
	score1 = score2 = 0;
	gameStates = intro;
	invincible = false;
	noDeath = false;

	menu = new mainMenu();
	menu->initialize(graphics, input); //, menuText);

	cheat = new cheatsMenu();
	cheat->initialize(graphics, input);

	creditsMenu = new genericMenu("Explosion and laser sounds by dklon from OpenGameArt.org","Background by Downdate from OpenGameArt.org","");
	creditsMenu->initialize(graphics, input);

	controlsMenu = new genericMenu("Press Arrow Keys to Move","Press Space to Shoot","");
	controlsMenu->initialize(graphics, input);

	waveFont = new TextDX();
	if(waveFont->initialize(graphics, 15, true, false, "Arial") == false)
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing DirectX font"));

	if (!bgTM.initialize(graphics,BG_IMAGE))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space texture"));
	if (!bgTexture.initialize(graphics,0,0,0,&bgTM))
        throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing space"));
    bgTexture.setScale(BG_SCALE);

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
	player.setLives(3);
	timeSinceHit = time(0);
			

	if (!healthTM.initialize(graphics,HEALTH_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player texture"));
	if (!health.initialize(this, HEALTH_WIDTH, HEALTH_HEIGHT, HEALTH_COLS,&healthTM))
		throw(GameError(gameErrorNS::WARNING, "Player not initialized"));
	health.setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT-2*health.getHeight()));
	health.setCollisionType(entityNS::BOX);
	health.setScale(1.0);
	health.setCurrentFrame(HEALTH_FULL);

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
		grunts[i].setInvisible();
		grunts[i].setCurrentFrame(GRUNT_IDLE_START);
		grunts[i].setFrameDelay(.008);
	}


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

	return;
}

//=============================================================================
// Update all game items
//=============================================================================

void CollisionTypes::gameStateUpdate()
{

	timeInState += frameTime;
	//main menu
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

	if (gameStates== wave1 && timeInState > 30)
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
	}
	if(gameStates == wave2 && player.getHealth() <= 0)
	{
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
	}


	if (gameStates==end && timeInState > 4)
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
	if (input->isKeyDown(VK_ESCAPE)) {
		exitGame();
	}
	gameStateUpdate();
	menu->update();
	cheat->update();
	creditsMenu->update();
	controlsMenu->update();

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
		if(timeSinceSpawn > 3)
		{
			grunts[++lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			grunts[++lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			timeSinceSpawn = 0;
			if(timeInState > 15) {
				grunts[++lastGrunt].spawn();	
				grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
				grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			}
			if(lastGrunt >= NUMGRUNTS-3)
				lastGrunt = 1;
		}
		if(input->isKeyDown(VK_LEFT))
			player.left();
		if(input->isKeyDown(VK_RIGHT))
			player.right();
		if(input->isKeyDown(VK_UP))
			player.up();
		if(input->isKeyDown(VK_DOWN))
			player.down();
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
		player.update(frameTime);
		laser.update(frameTime, player, audio);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		health.update(frameTime);
		//zep.update(frameTime);
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
		if(timeSinceSpawn > 2 && !bossSpawn)
		{
			//lastGrunt++;
			grunts[++lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			grunts[++lastGrunt].spawn();	
			grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
			grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			timeSinceSpawn = 0;
			if(timeInState > 15) {
				grunts[++lastGrunt].spawn();	
				grunts[lastGrunt].setCurrentFrame(GRUNT_IDLE_START);
				grunts[lastGrunt].setFrames(GRUNT_IDLE_START, GRUNT_IDLE_END);
			}
			if(lastGrunt >= NUMGRUNTS-3)
				lastGrunt = 1;
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
		if(input->isKeyDown(VK_UP))
			player.up();
		if(input->isKeyDown(VK_DOWN))
			player.down();
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
		player.update(frameTime);
		laser.update(frameTime, player, audio);
		for(int i = 0; i < NUMGRUNTS; i++) {
			grunts[i].update(frameTime);
		}
		zep.update(frameTime);
		health.update(frameTime);
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
		grunts[i].ai(frameTime, player);
		for(int j = 0; j < NUMGRUNTS; j++)
		{
			if(j != i)
				grunts[i].evade(grunts[j]);
		}
	}
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

		if(grunts[i].isHitBy(laser) && !grunts[i].getDead() && !laser.getDead()){
			grunts[i].setFrames(GRUNT_EXPLODE_START, GRUNT_EXPLODE_END);
			grunts[i].setDead(true);
			laser.setDead(true);
			laser.setVisible(false);
			audio->playCue(BOOM9);
			switch(gameStates) {
			case wave1:
				score1 += grunts[i].getScore();
				break;
			case wave2:
				score2+= grunts[i].getScore();
				break;
			}
			//grunts[i].setDead(true);
		}
		if(grunts[i].getCurrentFrame() == GRUNT_EXPLODE_END)
			grunts[i].setInvisible();
	}

	if(zep.collidesWith(player) && !zep.getDead()){
			zep.setCollides(true);
			player.setHealth(player.getHealth()-20);
			player.setX(240);
			player.setY(100);
		}
		else
			zep.setCollides(false);

		if(zep.isHitBy(laser) && !zep.getDead() && !laser.getDead()) {
			zep.setHealth(zep.getHealth() - 10);
			laser.setDead(true);
			laser.setVisible(false);
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
		if(zep.getCurrentFrame() == ZEP_EXPLODE_END)
			zep.setInvisible();
}

//=============================================================================
// Render game items
//=============================================================================
void CollisionTypes::render()
{
	float x = bgTexture.getX();
    float y = bgTexture.getY();
	graphics->spriteBegin(); // begin drawing sprites
	switch (gameStates)
	{
	case intro:
		menu->displayMenu();
		break;
	case cheats:
		cheat->displayMenu();
		break;
	case credits:
		creditsMenu->displayMenu();
		break;
	case controls:
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
		waveFont->print(std::to_string(score1 + score2), 50, 50);
		if(timeInState < 3)
			waveFont->print("Wave 1 \n" + std::to_string(player.getLives()) + " Lives Remaining",310,100);
		if(difftime(time(0), timeSinceHit) < .2)
			player.draw(D3DCOLOR_RGBA(255,0,0,255));
		else 
			player.draw();
		laser.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		
		break;
	case wave2:
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
		waveFont->print(std::to_string(score1 + score2), 50, 50);
		if(timeInState < 3)
			waveFont->print("Wave 2 \n" + std::to_string(player.getLives()) + " Lives Remaining",310,100);
		if(difftime(time(0), timeSinceHit) < .2)
			player.draw(D3DCOLOR_RGBA(255,0,0,255));
		else 
			player.draw();
		laser.draw();
		for(int i = 0; i < NUMGRUNTS; i++)
			grunts[i].draw();
		if(zep.getVisible())
			zep.draw();
		break;
		//draw stuff
	case end:
		//gameOver.draw();
		waveFont->print("Your score is: " + std::to_string(score1 + score2),300,100);
		break;
	
	case gameEnd:
		waveFont->print("Your score is: " + std::to_string(score1 + score2),300,100);
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
	backTexture1.onLostDevice();
	zepTM.onLostDevice();
	healthTM.onLostDevice();
	gruntTM.onLostDevice();
	playerTM.onLostDevice();
	bgTM.onLostDevice();
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
	Game::resetAll();
	return;
}
