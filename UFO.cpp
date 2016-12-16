#include "UFO.h"
#include <time.h>
#include <cstdlib>
//=============================================================================
// default constructor
//=============================================================================
Ufo::Ufo() : Entity()
{
    spriteData.width = ufoNS::WIDTH;           
    spriteData.height = ufoNS::HEIGHT;
    spriteData.x = ufoNS::X;                   // location on screen
    spriteData.y = ufoNS::Y;
    spriteData.rect.bottom = ufoNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = ufoNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = ufoNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -ufoNS::HEIGHT/2;
	spriteData.scale = 1;
	active = true;
	speed = 50;
	sightDistance = 10;
	isDead = false;
	srand(time(NULL)); //Why did I put this here?
	health = 100;
	score = 300;

}

bool Ufo::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Ufo::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Ufo::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Ufo::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
	if(!isDead) {
	/*	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
		{
			setPosition(D3DXVECTOR2(0,getPositionY()));
		}*/
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

//hacked at this code to make it avoid other Ufos
void Ufo::evade(Entity &t)
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

void Ufo::deltaTrack()
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


void Ufo::vectorTrack()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	vel.x = 3;
	setVelocity(-vel);
}

void Ufo::ai(float time){
	if(timeSinceMove > 1.5)
	{
	//	setPosition(VECTOR2(100,100));
		setPosition(VECTOR2(GAME_WIDTH - (rand()% 150) -40, rand()%(GAME_HEIGHT-50)+25) );
	}
	timeSinceMove += time;
}



bool Ufo::collidesWith(Entity p) {
	//ufo box
	float ufoL = spriteData.x;
	float ufoR = spriteData.x+ufoNS::WIDTH*spriteData.scale;
	float ufoT = spriteData.y;
	float ufoB = spriteData.y+ufoNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (ufoL < pR && ufoR > pL && ufoT < pB && ufoB > pT);
}

bool Ufo::isHitBy(Entity p) {
	//ufo box
	float ufoL = spriteData.x;
	float ufoR = spriteData.x+ufoNS::WIDTH*spriteData.scale;
	float ufoT = spriteData.y;
	float ufoB = spriteData.y+ufoNS::HEIGHT*spriteData.scale;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (ufoL < pR && ufoR > pL && ufoT < pB && ufoB > pT);
}

void Ufo::spawn() {
	if(getDead())
	{
		setPosition(VECTOR2(200, 200));
	//	ufos[++lastufo].setX(GAME_WIDTH-ufoNS::WIDTH);
	//	ufos[lastufo].setY(40);
		setDead(false);
		setVisible();
	}
}