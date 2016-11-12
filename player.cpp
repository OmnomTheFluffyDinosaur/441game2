
#include "player.h"

//=============================================================================
// default constructor
//=============================================================================
Player::Player() : Entity()
{
    spriteData.width = playerNS::WIDTH;           // size of Ship1
    spriteData.height = playerNS::HEIGHT;
    spriteData.x = playerNS::X;                   // location on screen
    spriteData.y = playerNS::Y;
    spriteData.rect.bottom = playerNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = playerNS::WIDTH;
    velocity.x = 0;                             // velocity X
    velocity.y = 0;                             // velocity Y
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    collision = false;
    collisionType =entityNS::BOX;
    target = false;
	edge.bottom = -playerNS::HEIGHT/2;
	edge.top = playerNS::HEIGHT/2;
	edge.right = -playerNS::WIDTH/2;
	edge.left = playerNS::WIDTH/2;
}

//=============================================================================
// Initialize the Ship.
// Post: returns true if successful, false if failed
//=============================================================================
bool Player::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Player::update(float frameTime)
{
    Entity::update(frameTime);
	
	//D3DXVec2Normalize(&velocity, &velocity);

	spriteData.x += velocity.x * frameTime;
    spriteData.y += velocity.y * frameTime;
	velocity = D3DXVECTOR2(0,0);

	
    // wrap around screen
    if (spriteData.x > GAME_WIDTH)                  // if off screen right
        spriteData.x = GAME_WIDTH-playerNS::WIDTH; 
    else if (spriteData.x < 0)         // else if off screen left
        spriteData.x = 0;
	if (spriteData.y+playerNS::HEIGHT > GAME_HEIGHT)                  // if off screen bottom
        spriteData.y = GAME_HEIGHT-playerNS::HEIGHT; 
    else if (spriteData.y < 0)         // else if off screen top
        spriteData.y = 0;
}