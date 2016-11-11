#include "puck.h"

//=============================================================================
// default constructor
//=============================================================================
Puck::Puck() : Entity()
{
    spriteData.width = puckNS::WIDTH;           
    spriteData.height = puckNS::HEIGHT;
    spriteData.x = puckNS::X;                   // location on screen
    spriteData.y = puckNS::Y;
    spriteData.rect.bottom = puckNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = puckNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = puckNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;

	edge.bottom = -puckNS::HEIGHT/2;
	spriteData.scale = 1;

}

bool Puck::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
	directionY = -1;
	directionX = 1;
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));

}
void Puck::changeDirectionY()
{
	 directionY *= -1;
}
void Puck::changeDirectionX()
{
	 directionX *= -1;
}

void Puck::setMissedPosition() 
{
	setPosition(VECTOR2(GAME_WIDTH/2, GAME_HEIGHT/5));
	setX(Entity::getPositionX());
	setY(Entity::getPositionY());
}
//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Puck::update(float frameTime)
{
    Entity::update(frameTime);
	float foo =puckNS::SPEED_X * directionX * frameTime;
	Entity::incPositionX(foo);
	foo = puckNS::SPEED_Y * directionY * frameTime;
	Entity::incPositionY(foo);

	if (Entity::getPositionX()+spriteData.width > GAME_WIDTH)                 
        changeDirectionX(); 
    else if (Entity::getPositionX() < 0)        
        changeDirectionX();            
	else if (Entity::getPositionY() < 0) 
		changeDirectionY();
	else if (Entity::getPositionY()+spriteData.height > GAME_HEIGHT) 
		changeDirectionY();
	spriteData.x = Entity::getPositionX();
	spriteData.y = Entity::getPositionY();
}

