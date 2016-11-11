#include "grunt.h"

//=============================================================================
// default constructor
//=============================================================================
Grunt::Grunt() : Entity()
{
    spriteData.width = gruntNS::WIDTH;           
    spriteData.height = gruntNS::HEIGHT;
    spriteData.x = gruntNS::X;                   // location on screen
    spriteData.y = gruntNS::Y;
    spriteData.rect.bottom = gruntNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = gruntNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = gruntNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -gruntNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
	sightDistance = 10;
}

bool Grunt::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Grunt::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Grunt::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Grunt::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
	{
		setPosition(D3DXVECTOR2(0,getPositionY()));
	}
	if (getPositionX() < 0)
	{
		setPosition(D3DXVECTOR2(GAME_WIDTH-Image::getWidth()*Image::getScale(),getPositionY()));
	}
	if (getPositionY() + Image::getHeight()*Image::getScale() > GAME_HEIGHT)
	{
		setPosition(D3DXVECTOR2(getPositionX(),0));
	}
	if (getPositionY() < 0)
	{
		setPosition(D3DXVECTOR2(getPositionX(),GAME_WIDTH-Image::getHeight()*Image::getScale()));
	}

	velocity = D3DXVECTOR2(0,0);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
}

void Grunt::evade()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	float length = D3DXVec2Length(&vel);
	if (length < 200) {
		VECTOR2* foo = D3DXVec2Normalize(&vel,&vel);
		setVelocity(vel);
		return;
	}
	setVelocity(D3DXVECTOR2(0,0));
	return;
}

void Grunt::deltaTrack()
{
	VECTOR2 vel = D3DXVECTOR2(-1,-1);
	VECTOR2 targetCenter = targetEntity.getCenterPoint();
	if (getCenterPoint().y - targetCenter.y < sightDistance) {
		vel.y = 1;
	}
	/*if (getCenterPoint().y == targetCenter.y) {
		vel.y = 0;
	}*/
	if (getCenterPoint().x - targetCenter.x < sightDistance) {
		vel.x = 1;
	}
	/*if (getCenterPoint().x == targetCenter.x) {
		vel.x = 0;
	}*/
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	setVelocity(vel);
}
void Grunt::vectorTrack()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	setVelocity(-vel);
}

void Grunt::ai(float time, Entity &t)
{ 
	targetEntity = t;
	vectorTrack();
	//deltaTrack();	
	//evade();
	//evade();
	return;
}