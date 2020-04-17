#ifndef pd
#define pd

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include "hexMove.h"


class pid {
public:
	//Constructor
	pid(bool massage = false);
	//Methods
    void pidBalance();

private:
	

};
#endif