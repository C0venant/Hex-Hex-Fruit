#include "commandLine.h"

static int saveTime = 0;

commandLine::commandLine(bool massage){};

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