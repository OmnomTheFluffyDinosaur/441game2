#ifndef _SPARK_MANAGER_H                // Prevent multiple definitions if this 
#define _SPARK_MANAGER_H                // file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "spark.h"
#include "constants.h"
#include "textureManager.h"

class SparkManager
{
	private:
	Spark sparks[MAX_NUM_SPARKS];
	VECTOR2 velocity; //all SPARKs created using SetVisibleNSPARKs() will use this velocity
	VECTOR2 position; //all SPARKs created using SetVisibleNSPARKs() will use this position
	TextureManager tm;

	float SparkManager::getVariance();// returns a number between 50% and 150% for Spark variance

public:
	SparkManager();
	~SparkManager();
	void SparkManager::setInvisibleAllSparks();
	void SparkManager::setVisibleNSparks(int n);
	void SparkManager::setPosition(VECTOR2 pos) {position = pos;}
	void SparkManager::setVelocity(VECTOR2 vel) {velocity = vel;}
	bool SparkManager::initialize(Graphics *g);

	void SparkManager::update(float frametime);
	void SparkManager::draw();


};







#endif _SPARK_MANAGER_H