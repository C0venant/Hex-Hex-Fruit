#ifndef cl
#define cl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include "BluetoothSerial.h"
#include "hexMove.h"
BluetoothSerial SerialBT;
hexMove move;


//incoming signals
#define balance 53
#define reset 58

class commandLine {
public:
	//Constructor
	commandLine(bool massage = false);

	//Methods
    void sendVoltage();
	void recieveCommand();

private:

};
#endif
