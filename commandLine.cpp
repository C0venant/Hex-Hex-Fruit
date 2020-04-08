#include "commandLine.h"

static int saveTime = 0;
bool toggle = true;

commandLine::commandLine(bool massage = false){};

void commandLine::sendVoltage(){
    if (millis() - saveTime > 3000) {
		saveTime = millis();
		int voltage = move.getVin();
		if (voltage > 1000) {
			uint8_t val = voltage/100;
			SerialBT.write(val);
		}
	}
}

void commandLine::recieveCommand(){
	if (SerialBT.available()) {
		BLUE blue;
		blue.command = SerialBT.read();
		Serial.println(blue.command);
		if (blue.command == balance) {
			toggle = false;
		}
		else if (toggle) {
			if (blue.command == changeHeight) {
				//blue.value = SerialBT.read();
				Serial.println(SerialBT.read());
			}
			//move.processCommand(blue);
		}
		else if (blue.command == reset) {
			ESP.restart();
		}
	}
}

