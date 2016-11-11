
              // file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#ifndef PATTERN_STEM_H                 // Prevent multiple definitions if this 
#define PATTERN_STEM_H   



//class PatternStep;

#include "constants.h"
#include "entity.h"
#include <cmath>
#include <d3d9.h>
#include <d3dx9.h>
#include "grunt.h"

namespace patternStepNS
{
}

// inherits from Entity class
class PatternStep
{
private:
   // puckNS::DIRECTION direction;    
    bool active;                  
	float timeInStep; //accumulates the time step has executed
	Grunt *entity;
	float timeForStep; //limit on the time for the step
	PATTERN_STEP_ACTION action;

public:
    // constructor
    PatternStep();

    // inherited member functions
    void initialize(Grunt *e);

    void update(float frameTime);

	void setEntity(Grunt *e) {entity = e;}
	void setAction(PATTERN_STEP_ACTION a) {action = a;}
	void setTimeForStep(float time) {timeForStep = time;}
	bool isFinished() {return !active;}
	void setActive() {active = true;}
	void setInactive() {active = false;}

};
#endif



