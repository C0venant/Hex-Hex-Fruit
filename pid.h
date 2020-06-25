#ifndef pd
#define pd

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include "hexMove.h"
#include "commandLine.h"
#include <Wire.h>
#include <PID_v1.h>
#include <MPU6050_tockn.h>

class pid {
public:
	//Constructor
	pid(bool massage = false);
	//Methods
    void pidBalance();
	void printConrdinates();
	void begin();
private:
	

};
#endif