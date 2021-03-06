//Particle System

#ifndef _SMOKE_H 
#define _SMOKE_H


#define WIN32_LEAN_AND_MEAN

class Smoke;

#include "image.h"
#include "input.h"
#include "game.h"

namespace smokeNS
{
}

class Smoke : public Image
{
	// Entity properties
private:
	//VECTOR2 position;
	bool active;
	VECTOR2 velocity;
	float timeAlive;
	float maxTimeAlive;
	float fadeValue; //1: opaque, 0: transparent
	float scaleValue;
	float rotationValue; //in radians


public:
	// Constructor
	Smoke();

	////////////////////////////////////////
	//           Get functions            //
	////////////////////////////////////////


	const VECTOR2 getVelocity() const {return velocity;}

	// Return active.
	bool  getActive()          {return active;}
	float getTimeAlive() {return timeAlive;}
	float getRotationValue() {return rotationValue;}
	float getMaxTimeAlive() {return maxTimeAlive;}
	bool getVisible() {return visible;}

	////////////////////////////////////////
	//           Set functions            //
	////////////////////////////////////////

	// Set velocity.
	void  setVelocity(VECTOR2 v)    {velocity = v;}
	void  setActive(bool a)         {active = a;}
	void setMaxTimeAlive(float t) {maxTimeAlive = t;}
	void setRotationValue(float r) {rotationValue = r;}
	void setVisible(bool b) {visible = b;}


	////////////////////////////////////////
	//         Other functions            //
	////////////////////////////////////////

	void update(float frameTime);
	bool initialize(Graphics *g, int width, int height, int ncols,
		TextureManager *textureM);
	void resetParticle();

};

#endif
