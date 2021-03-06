#include "zeppelin.h"

//=============================================================================
// default constructor
//=============================================================================
Zeppelin::Zeppelin() : Entity()
{
    spriteData.width = zepNS::WIDTH;           
    spriteData.height = zepNS::HEIGHT;
    spriteData.x = zepNS::X;                   // location on screen
    spriteData.y = zepNS::Y;
    spriteData.rect.bottom = zepNS::HEIGHT/2;    // rectangle to select parts of an image
    spriteData.rect.right = zepNS::WIDTH;
	velocity = D3DXVECTOR2(0,0);
    startFrame = 0;              // first frame of ship animation
    endFrame     = 0;              // last frame of ship animation
    currentFrame = startFrame;
    radius = zepNS::WIDTH/2.0;                 // collision radius
    collision = false;
    collisionType =entityNS::BOX;// entityNS::CIRCLE;
    target = false;
	edge.bottom = -zepNS::HEIGHT/2;
	spriteData.scale = 2;
	active = true;
	speed = 10;
	sightDistance = 10;
	isDead = false;
	health = 100;
	score = 500;
}

bool Zeppelin::initialize(Game *gamePtr, int width, int height, int ncols,
    TextureManager *textureM)
{
    return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Zeppelin::setInvisible()
{
	Image::setVisible(false);
	active = false;
}

void Zeppelin::setVisible()
{
	Image::setVisible(true);
	active = true;
}

//=============================================================================
// update
// typically called once per frame
// frameTime is used to regulate the speed of movement and animation
//=============================================================================
void Zeppelin::update(float frameTime)
{
	VECTOR2 foo = velocity*frameTime*speed;
	if(!isDead) {
	/*	if (getPositionX() + Image::getWidth()*Image::getScale() > GAME_WIDTH)
		{
			setPosition(D3DXVECTOR2(0,getPositionY()));
		}*/
		if (getPositionX() + zepNS::WIDTH*Image::getScale() < 0)
		{
			setPosition(D3DXVECTOR2(GAME_WIDTH,getPositionY()));
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

void Zeppelin::evade()
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

void Zeppelin::deltaTrack()
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


void Zeppelin::vectorTrack()
{
	VECTOR2 vel = getCenterPoint() - targetEntity.getCenterPoint();
	VECTOR2* foo = D3DXVec2Normalize(&vel, &vel);
	vel.x = 3;
	setVelocity(-vel);
}

void Zeppelin::ai(float time, Entity &t)
{ 
	targetEntity = t;
	vectorTrack();
	//deltaTrack();	
	//evade();
	//evade();
	return;
}

bool Zeppelin::collidesWith(Entity p) {
	//zeppelin box
	float zeppelinL = spriteData.x +32;
	float zeppelinR = spriteData.x+zepNS::WIDTH*spriteData.scale -32;
	float zeppelinT = spriteData.y +16;
	float zeppelinB = spriteData.y+zepNS::HEIGHT*spriteData.scale -16;

	//player box
	float pL = p.getX() + 16;
	float pR = p.getX()+p.getWidth()*spriteData.scale -16;
	float pT = p.getY() + 8;
	float pB = p.getY()+p.getHeight()*spriteData.scale - 8;

	return (zeppelinL < pR && zeppelinR > pL && zeppelinT < pB && zeppelinB > pT);
}

bool Zeppelin::isHitBy(Entity p) {
	//zeppelin box
	float zeppelinL = spriteData.x + 20;
	float zeppelinR = spriteData.x+zepNS::WIDTH*spriteData.scale-20;
	float zeppelinT = spriteData.y + 12;
	float zeppelinB = spriteData.y+zepNS::HEIGHT*spriteData.scale-12;

	//player box
	float pL = p.getX();
	float pR = p.getX()+p.getWidth()*spriteData.scale;
	float pT = p.getY();
	float pB = p.getY()+p.getHeight()*spriteData.scale;

	return (zeppelinL < pR && zeppelinR > pL && zeppelinT < pB && zeppelinB > pT);
}

void Zeppelin::spawn() {
	if(getDead())
	{
		setPosition(VECTOR2(GAME_WIDTH-6, rand()%(GAME_HEIGHT-zepNS::HEIGHT-12)+8));
	//	grunts[++lastGrunt].setX(GAME_WIDTH-gruntNS::WIDTH);
	//	grunts[lastGrunt].setY(40);
		setDead(false);
		setVisible();
	}
}