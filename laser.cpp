#include "laser.h"
#include "math.h"
#include <vector>

//=============================================================================
// default constructor
//=============================================================================
Laser::Laser() : Entity()
{
    spriteData.x    = laserNS::X;              // location on screen
    spriteData.y    = laserNS::Y;
	spriteData.width = laserNS::WIDTH;           // size of boat1
    spriteData.height = laserNS::HEIGHT;
	spriteData.scale = 0.5;
    mass = laserNS::MASS;
	velocity.x = laserNS::SPEED;                           // velocity X 
	velocity.y = laserNS::SPEED;
	active = false;  
	collisionType = entityNS::BOX;
	direction = true;
	isDead = false;
	cR = 0;
	reloadTimer = 0;
}

void Laser::update(float frameTime, Player p, Audio* audio){
	reloadTimer -= frameTime;
	if (reloadTimer <= 0) {
		if(input->wasKeyPressed(VK_SPACE)){
			setVisible(true);
			spriteData.x = p.getX() +40;
			spriteData.y = p.getY() +10;
			velocity.x = 1200;
			reloadTimer = 0.4f;
		}
	}
	spriteData.x += velocity.x*frameTime;
}
