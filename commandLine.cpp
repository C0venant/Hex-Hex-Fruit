#include "commandLine.h"


#define voltageInterval 3000
#define commandInterval 500
#define nullCommand 10

uint8_t lastCommand = 10;
static int lastCommandTime = 0;
static int saveTime = 0;
static bool toggle = true;
static hexMove move;


// constructor
commandLine::commandLine(bool massage){
	move.arrayInit();
};

// executes basic commands

void commandLine::executeCommand(uint8_t id){
	for(int i = 0; i < move.cSize; i++){
		if(move.commands[i].id == id){
			move.commands[i].fun();
			break;
		}
	}
}

// executes commands for pid

void commandLine::executePidCommand(uint8_t id, uint16_t v){
	for(int i = 0; i < move.cSize; i++){
		if(move.pidCommands[i].id == id){
			move.pidCommands[i].fun(v);
			break;
		}
	}
}

//sends servo voltage in every 3 seconds

void commandLine::sendVoltage(BluetoothSerial& SerialBT){
    if (millis() - saveTime > voltageInterval) {
		saveTime = millis();
		int voltage = move.getVin();
		if (voltage > 1000) {
			uint8_t val = voltage/100;
			SerialBT.write(val);
		}
	}
}

// repeat last command untill termination command is recieved

void commandLine::loopCommand(){
	if(lastCommand != nullCommand && millis()-lastCommandTime > commandInterval){
		lastCommandTime = millis();
		Serial.print("lastCommand: ");
		Serial.println(lastCommand);
		executeCommand(lastCommand);
	}
}

// recieves incoming signal and calls method executioner

void commandLine::recieveCommand(BluetoothSerial& SerialBT){
	if (SerialBT.available()) {
		Blue blue;
		blue.command = SerialBT.read();
		Serial.print("command: ");
		Serial.println(blue.command);
		lastCommand = blue.command;
		lastCommandTime = millis();
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
	}else{
		loopCommand();
	}
}