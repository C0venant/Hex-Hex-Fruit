#ifndef hs
#define hs

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

//command Array size
#define comSize 12
#define pidComSize 5

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
#define changeHeightInc 57
#define changeHeightDec 58
#define rightSideWalk 47
#define leftSideWalk 48
#define stop 40

//pid signals
#define lowY 1
#define lowZ 2
#define highY 3
#define highZ 4

struct Leg {
	uint8_t up;
	uint8_t mid;
	uint8_t down;
};

////////////////////////////////////////////
typedef void (*cmdFunction)();
typedef void (*pidFunction)(uint16_t);
struct command{
	uint8_t id;
	cmdFunction fun;
};

struct pidCommand{
	uint8_t id;
	pidFunction fun;
};
////////////////////////////////////////////
class hexMove {
public:
	//Constructor
	hexMove(bool massage = false);
	uint8_t cSize = comSize;
	command commands[comSize];
	pidCommand pidCommands[pidComSize];
	//Methods
	void legSetup();
	//void stopAll();
	void pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v);
	int getVin();
	void arrayInit();

private:
	void changeH(int delta, uint16_t v);
};

#endif