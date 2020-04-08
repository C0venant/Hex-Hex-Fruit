#ifndef hs
#define hs

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

//incoming signals
#define inPose 54
#define forward 50
#define right 51
#define left 49
#define backwards 52
#define wave 56
#define changeHeight 57

struct BLUE {
	uint8_t command;
	int16_t value;
};


class hexMove {
public:
	//Constructor
	hexMove(bool massage = false);

	//Methods
	void stopAll();
	void intialPos(int16_t pos3, int16_t pos2, int16_t pos1);
	void pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v);
	int getVin();
	void processCommand(BLUE blue);
private:
	void walking(uint8_t blue);
	void waveHand();
	void changeH(int16_t delta, uint16_t v);
};






#endif