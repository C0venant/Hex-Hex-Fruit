#ifndef cl
#define cl

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif


#include "quadMove.h"
#include "BluetoothSerial.h"


//incoming signals
#define balance 53
#define reset 60

#define hexMode 31
#define quadMode 30


class commandLine {
public:
	//Constructor
	commandLine(bool massage = false);

	//Methods
	bool balanceStatus();
	void functionsInit();
	void executeCommand(uint8_t id);
	void executePidCommand(uint8_t id, uint16_t v);
    void sendVoltage(BluetoothSerial& SerialBT);
	void recieveCommand(BluetoothSerial& SerialBT);
	void loopCommand();

private:
	

};
#endif
