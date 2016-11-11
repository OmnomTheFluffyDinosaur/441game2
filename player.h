
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef _PLAYER_H                 // Prevent multiple definitions if this 
#define _PLAYER_H   
class Player;

#include "entity.h"
#include "constants.h"

namespace playerNS
{
    const int WIDTH = 90;                   // image width
    const int HEIGHT = 20;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED = 120;             
	
   
}

// inherits from Entity class
class Player : public Entity
{
private:
  
    bool collision;                 
    bool target;                    

public:
    // constructor
    Player();

    // inherited member functions
    virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
                            TextureManager *textureM);
    void update(float frameTime);

    // Set collision Boolean
    void setCollision(bool c)
    {collision = c;}

    // Set collision type (NONE, CIRCLE, BOX, ROTATED_BOX)
    virtual void setCollisionType(entityNS::COLLISION_TYPE ctype)
    {collisionType = ctype;}

    // Set RECT structure used for BOX and ROTATED_BOX collision detection.
    void setEdge(RECT e) {edge = e;}

    // Set target
    void setTarget(bool t) {target = t;}

    // Get collision
    bool getCollision() {return collision;}

    // Get collision type
    entityNS::COLLISION_TYPE getCollisionType() {return collisionType;}

    void right()
    {
        velocity.x = playerNS::SPEED;
    }
    void left()
    {
        velocity.x = -playerNS::SPEED;
    }
	void up()
    {
        velocity.y = -playerNS::SPEED;
    }
    void down()
    {
        velocity.y = playerNS::SPEED;
    }

};
#endif

