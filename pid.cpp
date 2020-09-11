#include "pid.h"


//gyro
MPU6050 mpu6050(Wire);
commandLine pidCmd;

//PID variables
static double SetpointZ, InputZ, OutputZ1, OutputZ2;
static double SetpointY, InputY, OutputY1, OutputY2;
//initial tuning parameters
double Kp = 1, Ki = 0, Kd = 0;

PID zPos(&InputZ, &OutputZ1, &SetpointZ, Kp, Ki, Kd, REVERSE);
PID zNeg(&InputZ, &OutputZ2, &SetpointZ, Kp, Ki, Kd, DIRECT);
PID yPos(&InputY, &OutputY1, &SetpointY, Kp, Ki, Kd, REVERSE);
PID yNeg(&InputY, &OutputY2, &SetpointY, Kp, Ki, Kd, DIRECT);


//pid switch
bool enable = true;

int startTime = 0;

pid::pid(bool massage){}

void pid::begin(){
	Wire.begin();
	mpu6050.begin();
    mpu6050.calcGyroOffsets(true);
	SetpointZ = 122;
	SetpointY = 122;
	yPos.SetMode(AUTOMATIC);
	yPos.SetTunings(Kp, Ki, Kd);
	yNeg.SetMode(AUTOMATIC);
	yNeg.SetTunings(Kp, Ki, Kd);
	zPos.SetMode(AUTOMATIC);
	zPos.SetTunings(Kp, Ki, Kd);
	zNeg.SetMode(AUTOMATIC);
	zNeg.SetTunings(Kp, Ki, Kd);
}


void PidLoopY() {
	mpu6050.update();
	int angleY = -1*mpu6050.getAngleY();
	InputY = map(angleY, 50, 150, 0, 255);
	//yPos.Compute();
	//yNeg.Compute();
	OutputY1 = map(InputY, 0, 255, 500, 200);
	OutputY2 = map(InputY, 0, 255, 500, 200);
	if (angleY < 85) {
		pidCmd.executePidCommand(lowY, OutputY2);
	}
	else if (angleY > 95) {
        pidCmd.executePidCommand(highY, OutputY1);
	}
	else {
		pidCmd.executeCommand(stop);
		enable = false;
	}
}

void PidLoopZ() {
	mpu6050.update();
	int angleZ = mpu6050.getAngleZ();
	InputZ = map(angleZ, -40, 40, 0, 255);
	//zPos.Compute();
	//zNeg.Compute();
	OutputZ1 = map(InputZ, 0, 255, 500, 200);
	OutputZ2 = map(InputZ, 0, 255, 500, 200);
	if (angleZ < -5) {
       pidCmd.executePidCommand(lowZ, OutputZ2);
	}
	else if (angleZ > 5) {
       pidCmd.executePidCommand(highZ, OutputZ1);
	}
	else {
		pidCmd.executeCommand(stop);
		enable = true;
	}
}

void pid::printConrdinates(){
	mpu6050.update();
	Serial.print("  x:  ");
	Serial.print(mpu6050.getAngleX());
	Serial.print("  y:  ");
	Serial.print(mpu6050.getAngleY());
	Serial.print("  z:  ");
	Serial.print(mpu6050.getAngleZ());
	Serial.print("  InputY:  ");
	Serial.print(InputY);
	Serial.print("  OutputY1:  ");
	Serial.print(OutputY1);
	Serial.print("  OutputY2:  ");
	Serial.print(OutputY2);
	Serial.print("  InputZ:  ");
	Serial.print(InputZ);
	Serial.print("  OutputZ1:  ");
	Serial.print(OutputZ1);
	Serial.print("  OutputZ2:  ");
	Serial.println(OutputZ2);	
}

void pid::pidBalance(){
	if (enable) {
		PidLoopY();
	}
	else {
		PidLoopZ();
	}
	//show current angle anligment
	
	if(millis()-startTime > 1000){
		startTime = millis();
		printConrdinates();
	}
	
}

