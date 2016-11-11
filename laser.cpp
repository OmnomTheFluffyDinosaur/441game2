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
	canShoot = true;
}

void Laser::update(float frameTime, Player p, Audio* audio){
	if(input->wasKeyPressed(VK_SPACE) && canShoot){
		setVisible(true);
		canShoot = false;
		spriteData.x = p.getX() +40;
		spriteData.y = p.getY() +10;
		velocity.x = 800;
	}
	spriteData.x += velocity.x*frameTime;
	if(spriteData.x > GAME_WIDTH || spriteData.x < 0)
		canShoot = true;
}
