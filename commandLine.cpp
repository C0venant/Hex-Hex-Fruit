#include "commandLine.h"


static int saveTime = 0;
static bool toggle = true;
static hexMove move;

commandLine::commandLine(bool massage){
	move.arrayInit();
};

void commandLine::executeCommand(uint8_t id){
	Serial.println(id);
	for(int i = 0; i < move.cSize; i++){
		if(move.commands[i].id == id){
			move.commands[i].fun();
			break;
		}
	}
}

void commandLine::sendVoltage(BluetoothSerial& SerialBT){
    if (millis() - saveTime > 3000) {
		saveTime = millis();
		int voltage = move.getVin();
		if (voltage > 1000) {
			uint8_t val = voltage/100;
			SerialBT.write(val);
		}
	}
}

void commandLine::recieveCommand(BluetoothSerial& SerialBT){
	if (SerialBT.available()) {
		Blue blue;
		blue.command = SerialBT.read();
		Serial.print("command: ");
		Serial.println(blue.command);
		if (blue.command == balance) {
			toggle = false;
		}
		else if (toggle) {
			if (blue.command == changeHeight) {
				blue.value = SerialBT.read();
				Serial.print("new val: ");
				Serial.println(blue.value);
			}
			executeCommand(blue.command);
		}
		else if (blue.command == reset) {
			ESP.restart();
		}
	}
}