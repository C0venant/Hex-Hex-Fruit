#ifndef qm
#define qm

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include "hexMove.h"

//initial position values
#define initialUp 500
#define initialMid 420
#define initialDown 70


//incoming signals
#define quadInPose 54
#define quadForward 50
#define quadRight 51
#define quadReft 49
#define quadBackwards 52
#define quadChangeHeight 57
#define quadStop 40
#define spreadArm 47
#define narrowArm 48
#define changeAngleUp 58
#define changeAngleDown 59
#define quadForward 50


#define qArrSize 10 

class quadMove {
public:
	//Constructor
    quadMove(bool massage = false);
    command commands[qArrSize];
	void arrayInit();


private:
	
};

#endif