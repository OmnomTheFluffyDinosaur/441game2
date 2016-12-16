
#define WIN32_LEAN_AND_MEAN
#ifndef UFO_H                 // Prevent multiple definitions if this 
#define UFO_H   
class Ufo;

#include "entity.h"
#include "constants.h"
#include "graphics.h"

namespace ufoNS
{
    const int WIDTH = 64;                   // image width
    const int HEIGHT = 32;                  // image height
    const int X = GAME_WIDTH/2 - WIDTH/2;   // location on screen
    const int Y = GAME_HEIGHT/2 - HEIGHT/2;
    const float SPEED_X = 0;                
	const float SPEED_Y = -0;
 
}

// inherits from Entity class
class Ufo : public Entity
{
private:
   // puckNS::DIRECTION direction;    
    bool collision;                 
    bool target;  
	int directionX;
	int directionY;
	VECTOR2 velocity;
	float speed;
	Entity targetEntity;
	int sightDistance;
	bool direction;
	bool isDead;
	bool isHit;
	int score;
	float timeSinceMove;
public:
    // constructor
    Ufo();

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

	void setInvisible();

	void setVisible();

	void setVelocity(VECTOR2 v) {velocity = v;}

	VECTOR2 getVelocity() {return velocity;}

	void ai(float time);

	bool collidesWith(Entity p);

	bool isHitBy(Entity p);

	bool needEvade;
	//void spawn();

	void vectorTrack();
	void deltaTrack();
	void evade(Entity &t);
	void setCollides(bool c){isHit = c;}
	void setDead(bool c){isDead = c;}
	bool getDead(){return isDead;}
	int getScore(){return score;}
	void spawn();
};
#endif

