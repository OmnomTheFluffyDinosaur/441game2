#include "smokeManager.h"
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

SmokeManager::SmokeManager()
{
	srand(time(NULL));
}
SmokeManager::~SmokeManager()
{
}
float SmokeManager::getVariance()
{
	float foo = (rand() );
	foo = ((int)foo	% 100)/100.0f;
	foo += 0.5f;
	return foo;
}
void SmokeManager::setInvisibleAllParticles()
{
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		particles[i].setVisible(false);
		particles[i].setActive(false);
	}
}
void SmokeManager::setVisibleNParticles(int n)
{
	int activatedParticles = 0;
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		if (!particles[i].getActive()) //found an inactive particle
		{
			particles[i].setActive(true);
			particles[i].setMaxTimeAlive(MAX_SMOKE_LIFETIME*getVariance());
			float newX = velocity.x * getVariance(); 
			float newY = velocity.y  * getVariance();
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

bool SmokeManager::initialize(Graphics *g)
{
	if (!tm.initialize(g, SMOKE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing smoke texture"));
	for (int i = 0; i < MAX_NUM_SMOKES; i++)
	{
		if (!particles[i].initialize(g,0,0,0,&tm))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing smoke"));
		particles[i].setActive(false);
		particles[i].setVisible(false);
		particles[i].setScale(0.03f);
		particles[i].setRotationValue(0.1f);
	}
	return true;
}

void SmokeManager::update(float frametime)
{
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++){
		if (particles[i].getActive())
			particles[i].update(frametime);

	}
}

void SmokeManager::draw()
{
	byte fadeAmount;
	COLOR_ARGB color;
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++)
	{
		if (!particles[i].getActive())
			continue;
		float foo = particles[i].getMaxTimeAlive();  //MAX_PARTICLE_LIFETIME;
		float bar = particles[i].getTimeAlive();
		float foobar = (foo-bar)/foo;
		fadeAmount = 255 * foobar;
		color = D3DCOLOR_ARGB(fadeAmount,0xff,0xff,0xff);//fadeAmount,fadeAmount,fadeAmount);
		particles[i].draw(color);
		if (fadeAmount <= 20)
			particles[i].resetParticle();
	}
}