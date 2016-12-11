#include "med.h"
#include <time.h>
#include <cstdlib>
//=============================================================================
// default constructor
//=============================================================================
medPack::medPack() : Entity()
{
    spriteData.width = medPackNS::WIDTH;           
    spriteData.height = medPackNS::HEIGHT;
    spriteData.x = medPackNS::X;                   // location on screen
    spriteData.y = medPackNS::Y;
    spriteData.rect.bottom = medPackNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = medPackNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = medPackNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -medPackNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
	sightDistance = 10;
	isDead = false;
	srand(time(NULL)); //Why did I put this here?
	score = 100;

}

bool medPack::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void medPack::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void medPack::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void medPack::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
	if(!isDead) {
	/*	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
		{
			setPosition(D3DXVECTOR2(0,getPositionY()));
		}*/
		if (getPositionX() < 0)
		{
			setDead(true);
			setInvisible();
		}
		if (getPositionY() + Image::getHeight()*Image::getScale() > GAME_HEIGHT)
		{
			setPosition(D3DXVECTOR2(getPositionX(),0));
		}
		if (getPositionY() < 0)
		{
			setPosition(D3DXVECTOR2(getPositionX(),GAME_WIDTH-Image::getHeight()*Image::getScale()));
		}
	}
//	if (isDead) {
//		setInvisible();
//	}
//	else {
//		setVisible();
//	}

	velocity = D3DXVECTOR2(0,0);
	incPosition(foo);
	Image::setX(getPositionX());
	Image::setY(getPositionY());
    Entity::update(frameTime);
}

//hacked at this code to make it avoid other medPacks
void medPack::evade(Entity &t)
{
	VECTOR2 vel = getCenterPoint() - t.getCenterPoint();
	float length = D3DXVec2Length(&vel);
	if (length < 30) {
		VECTOR2* foo = D3DXVec2Normalize(&vel,&vel);
		setVelocity(vel);
		return;
	}
	//setVelocity(D3DXVECTOR2(0,0));
	return;
}

void medPack::deltaTrack()
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


void medPack::vectorTrack()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	vel.x = 3;
	setVelocity(-vel);
}

void medPack::ai()
{ 
	setVelocity(VECTOR2(-3,0));
	//targetEntity = t;
	//vectorTrack();
	//deltaTrack();	
	//evade();
	//evade();
	return;
}

bool medPack::collidesWith(Entity p) {
	//medPack box
	float medPackL = spriteData.x;
	float medPackR = spriteData.x+medPackNS::WIDTH*spriteData.scale;
	float medPackT = spriteData.y;
	float medPackB = spriteData.y+medPackNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (medPackL < pR && medPackR > pL && medPackT < pB && medPackB > pT);
}

bool medPack::isHitBy(Entity p) {
	//medPack box
	float medPackL = spriteData.x;
	float medPackR = spriteData.x+medPackNS::WIDTH*spriteData.scale;
	float medPackT = spriteData.y;
	float medPackB = spriteData.y+medPackNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (medPackL < pR && medPackR > pL && medPackT < pB && medPackB > pT);
}

void medPack::spawn() {
	if(getDead())
	{
		setPosition(VECTOR2(GAME_WIDTH-1, rand()%(GAME_HEIGHT-medPackNS::HEIGHT-50)+25) );
	//	medPacks[++lastmedPack].setX(GAME_WIDTH-medPackNS::WIDTH);
	//	medPacks[lastmedPack].setY(40);
		setDead(false);
		setVisible();
	}
}