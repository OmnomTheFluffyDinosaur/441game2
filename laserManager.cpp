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

bool LaserManager::initialize(Graphics *g, CollisionTypes *c)
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
		particles[i].setCol(c);
	}
	return true;
}

void LaserManager::update(float frametime)
{
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++){
		if (particles[i].getActive())
			particles[i].update(frametime);
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

bool LaserManager::collidesWith(Entity e) {
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++) {
		if (particles[i].getActive()) {
			float laserL = particles[i].getX();
			float laserR = particles[i].getX()+particles[i].getWidth()*particles[i].getScale();
			float laserT = particles[i].getY();
			float laserB = particles[i].getY()+particles[i].getHeight()*particles[i].getScale();

			//player box
			float eL = e.getX();
			float eR = e.getX()+e.getWidth()*e.getScale();
			float eT = e.getY();
			float eB = e.getY()+e.getHeight()*e.getScale();
			bool collided = (laserL < eR && laserR > eL && laserT < eB && laserB > eT);
			if (collided)  {
				particles[i].resetParticle();
				return collided;
			}
		}
	}
	return false;
}

void LaserManager::resetAll() {
	for (int i = 0; i < MAX_NUMBER_PARTICLES; i++) {
		particles[i].resetParticle();
	}
}