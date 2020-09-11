#include "commandLine.h"


#define voltageInterval 3000
#define commandInterval 500
#define nullCommand 10



static int WalkMode = hexMode;

uint8_t lastCommand = nullCommand;
static int lastCommandTime = 0;
static int saveTime = 0;
static hexMove move;
static quadMove qmove;
static uint8_t rCommand = 0;

bool balanceEnabled = false;


// constructor
commandLine::commandLine(bool massage){
	move.arrayInit();
	qmove.arrayInit();
};

bool commandLine::balanceStatus(){
	return balanceEnabled;
};

// executes basic commands

void commandLine::executeCommand(uint8_t id){
	if(WalkMode == hexMode){
		for(int i = 0; i < move.cSize; i++){
			if(move.commands[i].id == id){
				move.commands[i].fun();
				break;
			}
		}
	}else{
		for(int i = 0; i < qArrSize; i++){
			if(qmove.commands[i].id == id){
				qmove.commands[i].fun();
				break;
			}
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


void display(uint8_t command){
	Serial.print("Command: ");
	Serial.println(command);
}

// repeat last command untill termination command is recieved

void commandLine::loopCommand(){
	if(lastCommand != nullCommand && millis()-lastCommandTime > commandInterval){
		lastCommandTime = millis();
		display(lastCommand);
		executeCommand(lastCommand);
	}
}


void transform(){
	WalkMode = rCommand;
	lastCommand = nullCommand;
}

void toggleBalance(){
	if(balanceEnabled){
		balanceEnabled = false;
	}else{
		balanceEnabled = true;
	}
	lastCommand = nullCommand;
}

boolean checkForInternalChange(){
	if(rCommand == hexMode || rCommand == quadMode){
		transform();
		return true;
		
	}else if(rCommand == balance){
		toggleBalance();
		return true;
	}else if(rCommand == toggleLight){
		if(digitalRead(light) == LOW){
			digitalWrite(light, HIGH);
		}else{
			digitalWrite(light, LOW);
		}	
		return true;
	}
	return false;
}

// recieves incoming signal and calls method executioner

void commandLine::recieveCommand(BluetoothSerial& SerialBT){
	if (SerialBT.available()) {
		rCommand = SerialBT.read();
		display(rCommand);
		if(checkForInternalChange()){
			return;
		}
		lastCommand = rCommand;
		lastCommandTime = millis();
		executeCommand(rCommand);
		if (rCommand == reset) {
			ESP.restart();
		}
	}else{
		loopCommand();
	}
}