#include "busServo.h"
//#include <MPU6050_tockn.h>
//#include <Wire.h>
//#include <PID_v1.h>
//#include "commandLine.h"
#include "pid.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

//class objects
BluetoothSerial SerialBT;
//MPU6050 mpu6050(Wire);
commandLine cmd;
//hexMove move;

pid p;
//constants
#define RXD2 16
#define TXD2 17

/*

//PID variables
static double SetpointY, InputY, OutputY1, OutputY2;
static double SetpointX, InputX, OutputX1, OutputX2;
//initial tuning parameters
double Kp = 1, Ki = 0, Kd = 0;
PID yPos(&InputY, &OutputY1, &SetpointY, Kp, Ki, Kd, REVERSE);
PID yNeg(&InputY, &OutputY2, &SetpointY, Kp, Ki, Kd, DIRECT);
PID xPos(&InputX, &OutputX1, &SetpointX, Kp, Ki, Kd, REVERSE);
PID xNeg(&InputX, &OutputX2, &SetpointX, Kp, Ki, Kd, DIRECT);

//pid switch
bool enable = true;

*/

void legButtons() {
	pinMode(13, INPUT_PULLUP);
	pinMode(14, INPUT_PULLUP);
	pinMode(27, INPUT_PULLUP);
	pinMode(26, INPUT_PULLUP);
	pinMode(25, INPUT_PULLUP);
	pinMode(23, INPUT_PULLUP);
}

/*
// setup for PID
void pidInit() {
	SetpointY = 122;
	SetpointX = 122;
	xPos.SetMode(AUTOMATIC);
	xPos.SetTunings(Kp, Ki, Kd);
	xNeg.SetMode(AUTOMATIC);
	xNeg.SetTunings(Kp, Ki, Kd);
	yPos.SetMode(AUTOMATIC);
	yPos.SetTunings(Kp, Ki, Kd);
	yNeg.SetMode(AUTOMATIC);
	yNeg.SetTunings(Kp, Ki, Kd);
}
*/
/*
// starts balancing algorithm for x axis
void PidLoopX() {
	mpu6050.update();
	int angleX = mpu6050.getAngleZ();
	Serial.print("x:  ");
	Serial.print(angleX);
	InputX = map(angleX, -45, 45, 0, 255);
	xPos.Compute();
	xNeg.Compute();
	OutputX1 = map(OutputX1, 0, 255, 800, 400);
	OutputX2 = map(OutputX2, 0, 255, 800, 400);
	if (angleX < -5) {
		move.pidHeightControl(3, 770, 450, OutputX2);
		move.pidHeightControl(4, 770, 450, OutputX2);
		move.pidHeightControl(1, 420, 100, OutputX2);
		move.pidHeightControl(6, 420, 100, OutputX2);
	}
	else if (angleX > 5) {
		move.pidHeightControl(3, 420, 100, OutputX1);
		move.pidHeightControl(4, 420, 100, OutputX1);
		move.pidHeightControl(1, 770, 450, OutputX1);
		move.pidHeightControl(6, 770, 450, OutputX1);
	}
	else {
		move.stopAll();
		enable = true;
	}
}

// starts balancing algorithm for Y axis
void PidLoopY() {
	mpu6050.update();
	int angleY = -1 * mpu6050.getAngleY();
	Serial.print("  y:  ");
	Serial.println(angleY);
	InputY = map(angleY, 0, 180, 0, 255);
	yPos.Compute();
	yNeg.Compute();
	OutputY1 = map(OutputY1, 0, 255, 800, 400);
	OutputY2 = map(OutputY2, 0, 255, 800, 400);
	if (angleY < 75) {
		move.pidHeightControl(1, 420, 100, OutputY2);
		move.pidHeightControl(3, 420, 100, OutputY2);
		move.pidHeightControl(2, 420, 100, OutputY2);
		move.pidHeightControl(5, 770, 450, OutputY2);
		move.pidHeightControl(4, 770, 450, OutputY2);
		move.pidHeightControl(6, 770, 450, OutputY2);
	}
	else if (angleY > 85) {
		move.pidHeightControl(5, 420, 100, OutputY1);
		move.pidHeightControl(4, 420, 100, OutputY1);
		move.pidHeightControl(6, 420, 100, OutputY1);
		move.pidHeightControl(1, 770, 450, OutputY1);
		move.pidHeightControl(3, 770, 450, OutputY1);
		move.pidHeightControl(2, 770, 450, OutputY1);
	}
	else {
		move.stopAll();
		enable = false;
	}
}
*/

void checkOnGroundTest() {
	if (digitalRead(13) == LOW) {
		Serial.println("Pin 13 is high");
	}
	if(digitalRead(14) == LOW){
		Serial.println("Pin 14 is high");
	}
	if (digitalRead(27) == LOW) {
		Serial.println("Pin 27 is high");
	}
	if(digitalRead(26) == LOW){
		Serial.println("Pin 26 is high");
	}
	if (digitalRead(25) == LOW) {
		Serial.println("Pin 25 is high");
	}
	if(digitalRead(23) == LOW){
		Serial.println("Pin 23 is high");
	}

}

/*
void checkOnGround() {
	for (int i = 3;i <= 8;i++) {
		if (digitalRead(i) == HIGH) {
			move.pidHeightControl(i - 2, 770, 400, 500);
		}
		while (digitalRead(i) == HIGH) {
		}
		move.stopAll();
	}
}
*/

// begins serials on setup
void begins() {
	Serial.begin(115200);
	Serial2.begin(115200, SERIAL_8N1, RXD2, TXD2);
	SerialBT.begin("HEX-HEX FRUIT TEST"); //Bluetooth device name
	//Wire.begin();
	p.begin();
	//pinMode(19, OUTPUT);
	//pinMode(32, OUTPUT);
	//digitalWrite(19, LOW);
	//digitalWrite(32, LOW);
	
	//mpu6050.begin();
}

void setup() {
	begins();
	legButtons();
	cmd.executeCommand(inPose);
	delay(500);
	//mpu6050.calcGyroOffsets(true);
	//pidInit();
}



void loop() {
	checkOnGroundTest();
	if(cmd.balanceStatus()){
		p.pidBalance();
	}
	cmd.sendVoltage(SerialBT);
	cmd.recieveCommand(SerialBT);
	delay(20);
}
