#include "pid.h"


//gyro
MPU6050 mpu6050(Wire);
commandLine pidCmd;

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


pid::pid(bool massage){}

void pid::begin(){
	Wire.begin();
	mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
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


void PidLoopX() {
	//mpu6050.update();
	int angleX = mpu6050.getAngleZ();
	Serial.print("x:  ");
	Serial.print(angleX);
	InputX = map(angleX, -45, 45, 0, 255);
	xPos.Compute();
	xNeg.Compute();
	OutputX1 = map(OutputX1, 0, 255, 800, 400);
	OutputX2 = map(OutputX2, 0, 255, 800, 400);
	if (angleX < -5) {
		pidCmd.executePidCommand(lowX, OutputX1);
	}
	else if (angleX > 5) {
        pidCmd.executePidCommand(highX, OutputX1);
	}
	else {
		pidCmd.executeCommand(stop);
		enable = true;
	}
}

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
       pidCmd.executePidCommand(lowY, OutputX1);
	}
	else if (angleY > 85) {
       pidCmd.executePidCommand(highY, OutputX1);
	}
	else {
		pidCmd.executeCommand(stop);
		enable = false;
	}
}


void pid::pidBalance(){
	if (enable) {
		PidLoopY();
	}
	else {
		PidLoopX();
	}
}

