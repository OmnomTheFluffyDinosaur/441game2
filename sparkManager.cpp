#include "sparkManager.h"
#include <stdlib.h>
#include <time.h>
#include "graphics.h"

SparkManager::SparkManager()
{
	srand(time(NULL));
}
SparkManager::~SparkManager()
{
}
float SparkManager::getVariance()
{
	float foo = (rand() );
	foo = ((int)foo	% 500)/10.0f - 25;
	foo += 0.5f;
	return foo;
}
void SparkManager::setInvisibleAllSparks()
{
	for (int i = 0; i < MAX_NUM_SPARKS; i++)
	{
		sparks[i].setVisible(false);
		sparks[i].setActive(false);
	}
}
void SparkManager::setVisibleNSparks(int n)
{
	int activatedSparks = 0;
	for (int i = 0; i < MAX_NUM_SPARKS; i++)
	{
		if (!sparks[i].getActive()) //found an inactive spark
		{
			sparks[i].setActive(true);
			sparks[i].setMaxTimeAlive(MAX_SPARK_LIFETIME*getVariance()/10);
			float newX = velocity.x * getVariance(); 
			float newY = velocity.y  * getVariance();
			VECTOR2 v = VECTOR2(newX,newY);
			sparks[i].setX(position.x);
			sparks[i].setY(position.y);
			sparks[i].setVelocity(v);
			sparks[i].setVisible(true);
			activatedSparks++;
			if (activatedSparks == n)
				return;
		}
	}
}

bool SparkManager::initialize(Graphics *g)
{
	if (!tm.initialize(g, SPARK_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing spark texture"));
	for (int i = 0; i < MAX_NUM_SPARKS; i++)
	{
		if (!sparks[i].initialize(g,0,0,0,&tm))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing spark"));
		sparks[i].setActive(false);
		sparks[i].setVisible(false);
		sparks[i].setScale(0.4f);
		sparks[i].setRotationValue(0.1f);
	}
	return true;
}

void SparkManager::update(float frametime)
{
	for (int i = 0; i < MAX_NUM_SPARKS; i++){
		if (sparks[i].getActive())
			sparks[i].update(frametime);
		
	}
}

void SparkManager::draw()
{
	byte fadeAmount;
	COLOR_ARGB color;
	for (int i = 0; i < MAX_NUM_SPARKS; i++)
	{
		if (!sparks[i].getActive())
			continue;
	//	float foo = sparks[i].getMaxTimeAlive();  //MAX_spark_LIFETIME;
	//	float bar = sparks[i].getTimeAlive();
	//	float foobar = (foo-bar)/foo;
		fadeAmount = 128;// * foobar;
		color = D3DCOLOR_ARGB(fadeAmount,0xff,0xff,0xff);//fadeAmount,fadeAmount,fadeAmount);
		sparks[i].draw(color);
		if (fadeAmount <= 20)
			sparks[i].resetSpark();
	}
}