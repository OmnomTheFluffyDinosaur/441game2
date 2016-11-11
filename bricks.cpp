#include "bricks.h"

//=============================================================================
// default constructor
//=============================================================================
Brick::Brick() : Entity()
{
    spriteData.width = brickNS::WIDTH;           
    spriteData.height = brickNS::HEIGHT;
    spriteData.x = brickNS::X;                   // location on screen
    spriteData.y = brickNS::Y;
    spriteData.rect.bottom = brickNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = brickNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = brickNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -brickNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;

}

bool Brick::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Brick::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Brick::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Brick::update(float frameTime)
{
    Entity::update(frameTime);
}

	