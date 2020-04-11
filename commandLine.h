#ifndef cl
#define cl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include "hexMove.h"
#include "BluetoothSerial.h"


//incoming signals
#define balance 53
#define reset 58


class commandLine {
public:
	//Constructor
	commandLine(bool massage = false);

	//Methods
	void functionsInit();
	void executeCommand(uint8_t id);
    void sendVoltage(BluetoothSerial& SerialBT);
	void recieveCommand(BluetoothSerial& SerialBT);

private:
	

};
#endif
