#ifndef bs
#define bs

#if (ARDUINO >= 100)
#include "Arduino.h"
#else 
#include "WProgram.h"
#endif

#include <inttypes.h>

class busServo {
public:
	//Constructor
	busServo(bool massage = false);

	//Methods
	byte LobotCheckSum(byte buf[]);

	void serialMove(HardwareSerial& SerialX, uint8_t id, int16_t position, uint16_t time);

	void serialStop(HardwareSerial& SerialX, uint8_t id);

	void LobotSerialServoSetID(HardwareSerial& SerialX, uint8_t oldID, uint8_t newID);

	void LobotSerialServoSetMode(HardwareSerial& SerialX, uint8_t id, uint8_t Mode, int16_t Speed);

	void LobotSerialServoLoad(HardwareSerial& SerialX, uint8_t id);

	void LobotSerialServoUnload(HardwareSerial& SerialX, uint8_t id);

	int LobotSerialServoReceiveHandle(HardwareSerial& SerialX, byte* ret);

	int serialPos(HardwareSerial& SerialX, uint8_t id);

	int LobotSerialServoReadVin(HardwareSerial& SerialX, uint8_t id);

private:

};






#endif
