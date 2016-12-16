#ifndef _SMOKE_MANAGER_H                // Prevent multiple definitions if this 
#define _SMOKE_MANAGER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "smoke.h"
#include "constants.h"
#include "textureManager.h"

class SmokeManager
{
	private:
	Smoke particles[MAX_NUM_SMOKES];
	VECTOR2 velocity; //all particles created using SetVisibleNParticles() will use this velocity
	VECTOR2 position; //all particles created using SetVisibleNParticles() will use this position
	TextureManager tm;

	float SmokeManager::getVariance();// returns a number between 50% and 150% for particle variance

public:
	SmokeManager();
	~SmokeManager();
	void SmokeManager::setInvisibleAllParticles();
	void SmokeManager::setVisibleNParticles(int n);
	void SmokeManager::setPosition(VECTOR2 pos) {position = pos;}
	void SmokeManager::setVelocity(VECTOR2 vel) {velocity = vel;}
	bool SmokeManager::initialize(Graphics *g);

	void SmokeManager::update(float frametime);
	void SmokeManager::draw();


};







#endif _PARTICLE_MANAGER_H