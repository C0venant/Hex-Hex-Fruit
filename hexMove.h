#ifndef hs
#define hs

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

//command Array size
#define comSize 10


//initial position values
#define initialUp 500
#define initialMid 420
#define initialDown 70


//incoming signals
#define inPose 54
#define forward 50
#define right 51
#define left 49
#define backwards 52
#define wave 56
#define changeHeight 57
#define rightSideWalk 47
#define leftSideWalk 48

struct Blue {
	uint8_t command;
	int8_t value;
};

struct Leg {
	uint8_t up;
	uint8_t mid;
	uint8_t down;
};

////////////////////////////////////////////
typedef void (*cmdFunction)();
struct command{
	uint8_t id;
	cmdFunction fun;
};
////////////////////////////////////////////
class hexMove {
public:
	//Constructor
	hexMove(bool massage = false);
	uint8_t cSize = comSize;
	command commands[comSize];
	//Methods
	void legSetup();
	void stopAll();
	//void intialPos();
	void pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v);
	int getVin();
	void processCommand(Blue* blue);
	//void waveHand();
	void arrayInit();

private:
	//void walking(uint8_t blue);
	
	void changeH(int delta, uint16_t v);
};

#endif