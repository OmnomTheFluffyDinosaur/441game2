#include "point.h"
#include <time.h>
#include <cstdlib>
//=============================================================================
// default constructor
//=============================================================================
point::point() : Entity()
{
    spriteData.width = pointNS::WIDTH;           
    spriteData.height = pointNS::HEIGHT;
    spriteData.x = pointNS::X;                   // location on screen
    spriteData.y = pointNS::Y;
    spriteData.rect.bottom = pointNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = pointNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = pointNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -pointNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
	sightDistance = 10;
	isDead = false;
	srand(time(NULL)); //Why did I put this here?
	score = 100;

}

bool point::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void point::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void point::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void point::update(float frameTime)
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

//hacked at this code to make it avoid other points
void point::evade(Entity &t)
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

void point::deltaTrack()
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


void point::vectorTrack()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	vel.x = 3;
	setVelocity(-vel);
}

void point::ai()
{ 
	setVelocity(VECTOR2(-3,0));
	//targetEntity = t;
	//vectorTrack();
	//deltaTrack();	
	//evade();
	//evade();
	return;
}

bool point::collidesWith(Entity p) {
	//point box
	float pointL = spriteData.x;
	float pointR = spriteData.x+pointNS::WIDTH*spriteData.scale;
	float pointT = spriteData.y;
	float pointB = spriteData.y+pointNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (pointL < pR && pointR > pL && pointT < pB && pointB > pT);
}

bool point::isHitBy(Entity p) {
	//point box
	float pointL = spriteData.x;
	float pointR = spriteData.x+pointNS::WIDTH*spriteData.scale;
	float pointT = spriteData.y;
	float pointB = spriteData.y+pointNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (pointL < pR && pointR > pL && pointT < pB && pointB > pT);
}

void point::spawn() {
	if(getDead())
	{
		setPosition(VECTOR2(GAME_WIDTH-1, rand()%(GAME_HEIGHT-pointNS::HEIGHT-50)+25) );
	//	points[++lastpoint].setX(GAME_WIDTH-pointNS::WIDTH);
	//	points[lastpoint].setY(40);
		setDead(false);
		setVisible();
	}
}