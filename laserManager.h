#ifndef _LASER_MANAGER_H                // Prevent multiple definitions if this 
#define _LASER_MANAGER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "laser.h"
#include "constants.h"
#include "textureManager.h"
#include "entity.h"

class LaserManager
{
	private:
	Laser particles[MAX_NUMBER_PARTICLES];
	VECTOR2 velocity; //all particles created using SetVisibleNParticles() will use this velocity
	VECTOR2 position; //all particles created using SetVisibleNParticles() will use this position
	TextureManager tm;

	float LaserManager::getVariance();// returns a number between 50% and 150% for particle variance

public:
	LaserManager();
	~LaserManager();
	void LaserManager::setInvisibleAllParticles();
	void LaserManager::setVisibleNParticles(int n);
	void LaserManager::setPosition(VECTOR2 pos) {position = pos;}
	void LaserManager::setVelocity(VECTOR2 vel) {velocity = vel;}
	bool LaserManager::initialize(Graphics *g);

	void LaserManager::update(float frametime);
	void LaserManager::draw();
	bool LaserManager::collidesWith(Entity e);
	void LaserManager::resetAll();


};







#endif _PARTICLE_MANAGER_H