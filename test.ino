#include "busServo.h"
#include "pid.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//class objects
BluetoothSerial SerialBT;
commandLine cmd;
pid p;
//constants
#define RXD2 16
#define TXD2 17

void legButtons() {
	for (uint8_t i = 3;i <= 8;i++) {
		pinMode(i, INPUT_PULLUP);
	}
}

// begins serials on setup
void begins() {
	Serial.begin(115200);
	Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
	SerialBT.begin("HEX-HEX FRUIT TEST"); //Bluetooth device name
	p.begin();
	pinMode(light, OUTPUT);
	digitalWrite(light, HIGH);
}

void setup() {
	begins();
	cmd.executeCommand(inPose);
	delay(500);
}



void loop() {
	if(cmd.balanceStatus()){
		p.pidBalance();
	}
	cmd.sendVoltage(SerialBT);
	cmd.recieveCommand(SerialBT);
	delay(20);
}
