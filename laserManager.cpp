#include "laserManager.h"
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

LaserManager::LaserManager()
{
	srand(time(NULL));
}
LaserManager::~LaserManager()
{
}
float LaserManager::getVariance()
{
	float foo = (rand() );
	foo = ((int)foo	% 100)/100.0f;
	foo += 0.5f;
	return foo;
}
void LaserManager::setInvisibleAllParticles()
{
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		particles[i].setVisible(false);
		particles[i].setActive(false);
	}
}
void LaserManager::setVisibleNParticles(int n)
{
	int activatedParticles = 0;
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		if (!particles[i].getActive()) //found an inactive particle
		{
			particles[i].setActive(true);
			particles[i].setMaxTimeAlive(MAX_PARTICLE_LIFETIME);
			float newX = velocity.x; 
			float newY = velocity.y;
			VECTOR2 v = VECTOR2(newX,newY);
			particles[i].setX(position.x);
			particles[i].setY(position.y);
			particles[i].setVelocity(v);
			particles[i].setVisible(true);
			activatedParticles++;
			if (activatedParticles == n)
				return;
		}
	}
}

bool LaserManager::initialize(Graphics *g)
{
	if (!tm.initialize(g, LASER_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dust texture"));
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		if (!particles[i].initialize(g,0,0,0,&tm))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing dust"));
		particles[i].setActive(false);
		particles[i].setVisible(false);
		particles[i].setScale(2.0f);
		particles[i].setRotationValue(0.0f);
	}
	return true;
}

void LaserManager::update(float frametime)
{
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++){
		if (particles[i].getActive())
			particles[i].update(frametime);
		if (position.x > GAME_WIDTH-4)
			particles[i].resetParticle();
	}
}

void LaserManager::draw()
{
	COLOR_ARGB color;
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		if (!particles[i].getActive())
			continue;
		float foo = particles[i].getMaxTimeAlive();  //MAX_PARTICLE_LIFETIME;
		float bar = particles[i].getTimeAlive();
		float foobar = (foo-bar)/foo;
		color = D3DCOLOR_ARGB(0xff,0xff,0xff,0xff);//fadeAmount,fadeAmount,fadeAmount);
		particles[i].draw(color);
	}
}