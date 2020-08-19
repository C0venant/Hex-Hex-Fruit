#include "hexMove.h"
#include "busServo.h"

#define delayCoef 100
#define heightDelta 50

static bool stepTurn = true;
static busServo servo;
static uint8_t legButtons[6];
static bool buttonSwitch = false;
static Leg legs[6];

struct legPos
{
	int up;
	int mid;
	int down;
};

void hexMove::legSetup()
{
	legs[0] = {6, 5, 4};
	legs[1] = {15, 14, 13};
	legs[2] = {18, 17, 16};
	legs[3] = {12, 11, 10};
	legs[4] = {3, 2, 1};
	legs[5] = {9, 8, 7};
}

void hexMove::legButtonSetup()
{
	legButtons[0] = 23;
	legButtons[1] = 25;
	legButtons[2] = 26;
	legButtons[3] = 27;
	legButtons[4] = 14;
	legButtons[5] = 13;
}

hexMove::hexMove(bool massage)
{
	legSetup();
	legButtonSetup();
}

void getposition(uint8_t n, legPos *pos)
{
	pos->down = -2048;
	pos->mid = -2048;
	//pos->up = -2048;
	while (pos->down == -2048)
	{
		Serial.print("bageqsa");
		pos->down = servo.serialPos(Serial2, legs[n - 1].down);
	}
	while (pos->mid == -2048)
	{
		pos->mid = servo.serialPos(Serial2, legs[n - 1].mid);
	}
	/*
	while(pos->up == -2048){
		pos->up = servo.serialPos(Serial2, legs[n-1].up);
	}	
	*/
}

void toggleBSwitch()
{
	buttonSwitch = !buttonSwitch;
}

void stopLeg(int n)
{
	servo.serialStop(Serial2, legs[n].mid);
	servo.serialStop(Serial2, legs[n].down);
}

void waitTripodDown(int f, int s, int t)
{
	bool first = true;
	bool second = true;
	bool third = true;
	while (true)
	{
		if (!first && !second && !third)
			break;
		if (digitalRead(legButtons[f]) == LOW)
		{
			first = false;
			stopLeg(f);
		}
		if (digitalRead(legButtons[s]) == LOW)
		{
			second = false;
			stopLeg(s);
		}
		if (digitalRead(legButtons[t]) == LOW)
		{
			third = false;
			stopLeg(t);
		}
	}
}

void waitTripod1Down()
{
	waitTripodDown(0, 2, 4);
}

void waitTripod2Down()
{
	waitTripodDown(1, 3, 5);
}

void liftUp(uint8_t n, int16_t pos)
{
	servo.serialMove(Serial2, legs[n - 1].mid, pos, 200);
}

void liftDown(uint8_t n, int16_t pos)
{
	servo.serialMove(Serial2, legs[n - 1].mid, pos, 200);
}

// pushes body in horizontal direction
void horPush(uint8_t n, int16_t pos)
{
	servo.serialMove(Serial2, legs[n - 1].up, pos, 200);
}

void lean(uint8_t n, int16_t midpos, int16_t downpos)
{
	//servo.serialMove(Serial2, legs[n-1].mid, midpos, 200);
	servo.serialMove(Serial2, legs[n - 1].down, downpos, 200);
}

//change height of the body according to delta
//this is old one with status bar
void hexMove::changeH(int delta, uint16_t v)
{
	legPos pos[6];
	for (uint8_t i = 0; i < 6; i++)
	{
		getposition(i, &pos[i]);
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		servo.serialMove(Serial2, legs[i].mid, pos[i].mid + delta, v);
		servo.serialMove(Serial2, legs[i].down, pos[i].down + delta, v);
	}
}

//new ones with buttons

void changeHInc()
{
	legPos pos[6];
	//for the time being lets get from only one
	for (uint8_t i = 0; i < 1; i++)
	{
		getposition(i + 1, &pos[i]);
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		servo.serialMove(Serial2, legs[i].mid, pos[0].mid + heightDelta, 300);
		servo.serialMove(Serial2, legs[i].down, pos[0].down + heightDelta, 300);
	}
}

void changeHDec()
{
	legPos pos[6];
	//for the time being lets get from only one
	for (uint8_t i = 0; i < 1; i++)
	{
		getposition(i + 1, &pos[i]);
	}
	for (uint8_t i = 0; i < 6; i++)
	{
		servo.serialMove(Serial2, legs[i].mid, pos[0].mid - heightDelta, 300);
		servo.serialMove(Serial2, legs[i].down, pos[0].down - heightDelta, 300);
	}
}
/*
static void stepCheck(int a, int b, int c) {
	int one = digitalRead(a + 2);
	int two = digitalRead(b + 2);
	int three = digitalRead(c + 2);
	while (!(one || two || three)) {
		one = digitalRead(a + 2);
		two = digitalRead(b + 2);
		three = digitalRead(c + 2);
	}
}
*/
///////// this part of code contains helper functions for robot movement

void tripod1Up(int pos)
{
	liftUp(1, pos);
	liftUp(3, pos);
	liftUp(5, pos);
	delay(delayCoef);
}
void tripod2Up(int pos)
{
	liftUp(2, pos);
	liftUp(4, pos);
	liftUp(6, pos);
	delay(delayCoef);
}
void tripod1Down(int pos)
{
	liftDown(1, pos);
	liftDown(3, pos);
	liftDown(5, pos);
	delay(delayCoef);
}
void tripod2Down(int pos)
{
	liftDown(2, pos);
	liftDown(4, pos);
	liftDown(6, pos);
	delay(delayCoef);
}
void tripod1Push(int16_t pos)
{
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 + pos);
	delay(delayCoef);
}
void tripod2Push(int16_t pos)
{
	horPush(2, 500 - pos);
	horPush(4, 500 + pos);
	horPush(6, 500 + pos);
	delay(delayCoef);
}
void tripod1Turn(int16_t pos)
{
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 - pos);
	delay(delayCoef);
}
void tripod2Turn(int16_t pos)
{
	horPush(2, 500 - pos);
	horPush(4, 500 - pos);
	horPush(6, 500 - pos);
	delay(delayCoef);
}

void tripod1Lean(int initial, int16_t pos)
{
	lean(1, 0, initial + pos);
	lean(3, 0, initial + pos);
	lean(5, 0, initial - pos);
	delay(delayCoef);
}

void tripod2Lean(int initial, int16_t pos)
{
	lean(2, 0, initial - pos);
	lean(4, 0, initial + pos);
	lean(6, 0, initial + pos);
	delay(delayCoef);
}
///////////////////////////////////////////////////

///////////// this segment of code contains main functions for robot movement

// tripod1 step takes 1 for forward and -1 for backwards motion
void step1(int sign)
{
	legPos pos[1];
	getposition(1, &pos[0]);
	tripod1Up(pos[0].mid - 120);
	tripod2Push(0);
	tripod1Push(sign * 150);
	if (!buttonSwitch)
	{
		tripod1Down(pos[0].mid);
	}
	//stepCheck (1,3,5);
}

// tripod2 step takes 1 for forward and -1 for backwards motion
void step2(int sign)
{
	legPos pos[1];
	getposition(2, &pos[0]);
	tripod2Up(pos[0].mid - 120);
	tripod1Push(0);
	tripod2Push(sign * 150);
	if (!buttonSwitch)
	{
		tripod2Down(pos[0].mid);
	}
	//stepCheck (2,4,6);
}
//tripod1 turn
void turn1(int sign)
{
	legPos pos[1];
	getposition(1, &pos[0]);
	tripod1Up(pos[0].mid - 120);
	tripod2Turn(0);
	tripod1Turn(sign * 150);
	if (!buttonSwitch)
	{
		tripod1Down(pos[0].mid);
	}
	//stepCheck (1,3,5);
}
//tripod2 turn
void turn2(int sign)
{
	legPos pos[1];
	getposition(2, &pos[0]);
	tripod2Up(pos[0].mid - 120);
	tripod1Turn(0);
	tripod2Turn(sign * 150);
	if (!buttonSwitch)
	{
		tripod2Down(pos[0].mid);
	}
	//stepCheck (2,4,6);
}
//walk sideways
void sideWalk1(int sign)
{
	legPos pos[1];
	getposition(1, &pos[0]);
	tripod1Up(pos[0].mid - 120);
	tripod2Lean(pos[0].down, 0);
	tripod1Lean(pos[0].down, sign * 70);
	if (!buttonSwitch)
	{
		tripod1Down(pos[0].mid);
	}
}

static void sideWalk2(int sign)
{
	legPos pos[1];
	getposition(2, &pos[0]);
	tripod2Up(pos[0].mid - 120);
	tripod1Lean(pos[0].down, 0);
	tripod2Lean(pos[0].down, sign * 70);
	if (!buttonSwitch)
	{
		tripod2Down(pos[0].mid);
	}
}

void stepForward()
{
	if (stepTurn)
	{
		stepTurn = false;
		step1(1);
	}
	else
	{
		stepTurn = true;
		step2(1);
	}
}

void stepBackward()
{
	if (stepTurn)
	{
		stepTurn = false;
		step1(-1);
	}
	else
	{
		stepTurn = true;
		step2(-1);
	}
}

void turnLeft()
{
	if (stepTurn)
	{
		stepTurn = false;
		turn1(-1);
	}
	else
	{
		stepTurn = true;
		turn2(-1);
	}
}

void turnRight()
{
	if (stepTurn)
	{
		stepTurn = false;
		turn1(1);
	}
	else
	{
		stepTurn = true;
		turn2(1);
	}
}

void sideWalkLeft()
{
	if (stepTurn)
	{
		stepTurn = false;
		sideWalk1(1);
	}
	else
	{
		stepTurn = true;
		sideWalk2(-1);
	}
}

void sideWalkRight()
{
	if (stepTurn)
	{
		stepTurn = false;
		sideWalk1(-1);
	}
	else
	{
		stepTurn = true;
		sideWalk2(1);
	}
}
////////////////////////////////////////////
////this part of code contains functions for pid balance
// changes Height of legs for Pid
void pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v)
{
	servo.serialMove(Serial2, legs[n - 1].mid, midPos, v);
	servo.serialMove(Serial2, legs[n - 1].down, lowPos, v);
}

void stopAll()
{
	for (uint8_t i = 1; i <= 18; i++)
	{
		servo.serialStop(Serial2, i);
	}
}

void pidCaseLowY(uint16_t output)
{
	pidHeightControl(3, 420, 100, output);
	pidHeightControl(4, 420, 100, output);
	pidHeightControl(1, 770, 450, output);
	pidHeightControl(6, 770, 450, output);
}

void pidCaseHighY(uint16_t output)
{
	pidHeightControl(3, 770, 450, output);
	pidHeightControl(4, 770, 450, output);
	pidHeightControl(1, 420, 100, output);
	pidHeightControl(6, 420, 100, output);
}

void pidCaseLowZ(uint16_t output)
{
	pidHeightControl(1, 420, 100, output);
	pidHeightControl(3, 420, 100, output);
	pidHeightControl(2, 420, 100, output);
	pidHeightControl(5, 770, 450, output);
	pidHeightControl(4, 770, 450, output);
	pidHeightControl(6, 770, 450, output);
}

void pidCaseHighZ(uint16_t output)
{
	pidHeightControl(5, 420, 100, output);
	pidHeightControl(4, 420, 100, output);
	pidHeightControl(6, 420, 100, output);
	pidHeightControl(1, 770, 450, output);
	pidHeightControl(3, 770, 450, output);
	pidHeightControl(2, 770, 450, output);
}

//////////////////////////////////////////////////

void waveHand()
{
	servo.serialMove(Serial2, legs[0].mid, 200, 400);
	delay(100);
	servo.serialMove(Serial2, legs[0].down, 400, 400);
	delay(300);
	servo.serialMove(Serial2, legs[0].up, 600, 400);
	delay(600);
	servo.serialMove(Serial2, legs[0].up, 350, 400);
	delay(10);
}

void intialPos()
{
	for (int i = 3; i < 19; i += 3)
	{
		servo.serialMove(Serial2, i, initialUp, 600);
	}
	for (int i = 2; i < 18; i += 3)
	{
		servo.serialMove(Serial2, i, initialMid, 600);
	}
	for (int i = 1; i < 18; i += 3)
	{
		servo.serialMove(Serial2, i, initialDown, 600);
	}
}

int hexMove::getVin()
{
	return servo.LobotSerialServoReadVin(Serial2, 1);
}

void hexMove::arrayInit()
{
	commands[0] = {wave, waveHand};
	commands[1] = {forward, stepForward};
	commands[2] = {backwards, stepBackward};
	commands[3] = {right, turnRight};
	commands[4] = {left, turnLeft};
	commands[5] = {rightSideWalk, sideWalkRight};
	commands[6] = {leftSideWalk, sideWalkLeft};
	commands[7] = {inPose, intialPos};
	commands[8] = {stop, stopAll};
	commands[9] = {changeHeightInc, changeHInc};
	commands[10] = {changeHeightDec, changeHDec};
	commands[11] = {toggleButtonSwitch, toggleBSwitch};

	pidCommands[0] = {lowY, pidCaseLowY};
	pidCommands[1] = {lowZ, pidCaseLowZ};
	pidCommands[2] = {highY, pidCaseHighY};
	pidCommands[3] = {highZ, pidCaseHighZ};
}