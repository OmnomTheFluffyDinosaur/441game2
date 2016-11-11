
#ifndef _LASER_H               // Prevent multiple definitions if this 
#define _LASER_H               // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "entity.h"
#include "constants.h"
#include "player.h"
#include <vector>


namespace laserNS
{
    const int   WIDTH = 16;                // image width
    const int   HEIGHT = 8;               // image height
    const int   X = 0;
    const int   Y = 0;
    const float MASS = 1.0e14f;         // mass
	const float SPEED = 100;	
	const int TEXTURE_COLS = 1;
	const float SCALE = 0.5;
}

class Laser : public Entity            // inherits from Entity class
{
public:
    // constructor
    Laser();
	void update(float frameTime, Player p, Audio *audio);
	bool canShoot;
private:
	bool direction;
	bool isDead;
	int cR;
};
#endif