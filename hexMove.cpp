#include "hexMove.h"
#include "busServo.h"

#define delayCoef 100

static bool stepTurn = true;
static busServo servo;

struct legPos {
	int up;
	int mid;
	int down;
};
 
static Leg legs[6];

hexMove::hexMove(bool massage) {
	legs[0].up = 6;
	legs[0].mid = 5;
	legs[0].down = 4;

	legs[1].up= 15;
	legs[1].mid= 14;
	legs[1].down= 13;

	legs[2].up = 18;
	legs[2].mid = 17;
	legs[2].down = 16;

	legs[3].up = 12;
	legs[3].mid = 11;
	legs[3].down = 10;

	legs[4].up = 3;
	legs[4].mid = 2;
	legs[4].down = 1;

	legs[5].up = 9;
	legs[5].mid = 8;
	legs[5].down= 7;
}

static void getposition(uint8_t n, legPos* pos){
	pos->down = -2048;
	pos->mid = -2048;
	pos->up = -2048;
	while(pos->down == -2048){
		pos->down = servo.serialPos(Serial2, legs[n-1].down);
	}
	while(pos->mid == -2048){
		pos->mid = servo.serialPos(Serial2, legs[n-1].mid);
	}
	while(pos->up == -2048){
		pos->up = servo.serialPos(Serial2, legs[n-1].up);
	}	
}

static void liftUp(uint8_t n, int16_t pos) {
	servo.serialMove(Serial2, legs[n-1].mid, pos, 200);

}
static void liftDown(uint8_t n, int16_t pos) {
	servo.serialMove(Serial2, legs[n-1].mid, pos, 200);
}


// pushes body in horizontal direction
static void horPush(uint8_t n, int16_t pos) {
	servo.serialMove(Serial2, legs[n-1].up, pos, 200);
}

static void lean(uint8_t n, int16_t midpos, int16_t downpos){
	//servo.serialMove(Serial2, legs[n-1].mid, midpos, 200);
	servo.serialMove(Serial2, legs[n-1].down, downpos, 200);
}

//change height of the body according to delta
void hexMove::changeH(int delta, uint16_t v) {
	legPos pos[6];
	for (uint8_t i = 0; i < 6; i++){
		getposition(i, &pos[i]);
	}
	for (uint8_t i = 0; i < 6; i++){
		servo.serialMove(Serial2, legs[i].mid, pos[i].mid + delta, v);
		servo.serialMove(Serial2, legs[i].down, pos[i].down + delta, v);
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

static void tripod1Up() {
	liftUp(1, 300);
	liftUp(3, 300);
	liftUp(5, 300);
	delay(delayCoef);
}
static void tripod2Up() {
	liftUp(2, 300);
	liftUp(4, 300);
	liftUp(6, 300);
	delay(delayCoef);
}
static void tripod1Down() {
	liftDown(1, 420);
	liftDown(3, 420);
	liftDown(5, 420);
	delay(delayCoef);
}
static void tripod2Down() {
	liftDown(2, 420);
	liftDown(4, 420);
	liftDown(6, 420);
	delay(delayCoef);
}
static void tripod1Push(int16_t pos) {
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 + pos);
	delay(delayCoef);
}
static void tripod2Push(int16_t pos) {
	horPush(2, 500 - pos);
	horPush(4, 500 + pos);
	horPush(6, 500 + pos);
	delay(delayCoef);
}
static void tripod1Turn(int16_t pos) {
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 - pos);
	delay(delayCoef);
}
static void tripod2Turn(int16_t pos) {
	horPush(2, 500 - pos);
	horPush(4, 500 - pos);
	horPush(6, 500 - pos);
	delay(delayCoef);
}

static void tripod1Lean(int16_t pos){
	lean(1,0,70+pos);
	lean(3,0,70+pos);
	lean(5,0,70-pos);
	delay(delayCoef);
}

static void tripod2Lean(int16_t pos){
	lean(2,0,70-pos);
	lean(4,0,70+pos);
	lean(6,0,70+pos);
	delay(delayCoef);
}
///////////////////////////////////////////////////

///////////// this segment of code contains main functions for robot movement

// tripod1 step takes 1 for forward and -1 for backwards motion
static void step1(int sign) {
	tripod1Up();
	tripod2Push(0);
	tripod1Push(sign * 150);
	tripod1Down();
	//stepCheck (1,3,5);   
}

// tripod2 step takes 1 for forward and -1 for backwards motion
static void step2(int sign) {
	tripod2Up();
	tripod1Push(0);
	tripod2Push(sign * 150);
	tripod2Down();
	//stepCheck (2,4,6);   
}
//tripod1 turn 
static void turn1(int sign) {
	tripod1Up();
	tripod2Turn(0);
	tripod1Turn(sign * 150);
	tripod1Down();
	//stepCheck (1,3,5);   
}
//tripod2 turn
static void turn2(int sign) {
	tripod2Up();
	tripod1Turn(0);
	tripod2Turn(sign * 150);
	tripod2Down();
	//stepCheck (2,4,6);      
}
//walk sideways
static void sideWalk1(int sign){
	tripod1Up();
	tripod2Lean(0);
	tripod1Lean(sign*70);
	tripod1Down();
}

static void sideWalk2(int sign){
	tripod2Up();
	tripod1Lean(0);
	tripod2Lean(sign*70);
	tripod2Down();
}


void stepForward(){
	if (stepTurn){
		stepTurn = false;
		step1(1);
	}else{
		stepTurn = true;
		step2(1);
	}
}

void stepBackward(){
	if (stepTurn){
		stepTurn = false;
		step1(-1);
	}else{
		stepTurn = true;
		step2(-1);
	}
}

void turnLeft(){
	if (stepTurn){
		stepTurn = false;
		turn1(-1);
	}else{
		stepTurn = true;
		turn2(-1);
	}
}

void turnRight(){
	if (stepTurn){
		stepTurn = false;
		turn1(1);
	}else{
		stepTurn = true;
		turn2(1);
	}
}

void sideWalkLeft(){
	if (stepTurn){
		stepTurn = false;
		sideWalk1(1);
	}else{
		stepTurn = true;
		sideWalk2(-1);
	}
}

void sideWalkRight(){
	if (stepTurn){
		stepTurn = false;
		sideWalk1(-1);
	}else{
		stepTurn = true;
		sideWalk2(1);
	}
}
////////////////////////////////////////////
////this part of code contains functions for pid balance
// changes Height of legs for Pid
void pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v) {
	servo.serialMove(Serial2, legs[n-1].mid, midPos, v);
	servo.serialMove(Serial2, legs[n-1].down, lowPos, v);
}

void stopAll() {
	for (uint8_t i = 1;i <= 18;i++) {
		servo.serialStop(Serial2, i);
	}
}

void pidCaseLowX(uint16_t output){
	pidHeightControl(3, 770, 450, output);
	pidHeightControl(4, 770, 450, output);
	pidHeightControl(1, 420, 100, output);
	pidHeightControl(6, 420, 100, output);
}

void pidCaseHighX(uint16_t output){
	pidHeightControl(3, 420, 100, output);
	pidHeightControl(4, 420, 100, output);
	pidHeightControl(1, 770, 450, output);
	pidHeightControl(6, 770, 450, output);
}

void pidCaseLowY(uint16_t output){
	pidHeightControl(1, 420, 100, output);
	pidHeightControl(3, 420, 100, output);
	pidHeightControl(2, 420, 100, output);
	pidHeightControl(5, 770, 450, output);
	pidHeightControl(4, 770, 450, output);
	pidHeightControl(6, 770, 450, output);
}

void pidCaseHighY(uint16_t output){
	pidHeightControl(5, 420, 100, output);
	pidHeightControl(4, 420, 100, output);
	pidHeightControl(6, 420, 100, output);
	pidHeightControl(1, 770, 450, output);
	pidHeightControl(3, 770, 450, output);
	pidHeightControl(2, 770, 450, output);
}

//////////////////////////////////////////////////

void waveHand() {
	servo.serialMove(Serial2, legs[0].mid, 200, 400);
	delay(100);
	servo.serialMove(Serial2, legs[0].down, 400, 400);
	delay(300);
	servo.serialMove(Serial2, legs[0].up, 600, 400);
	delay(600);
	servo.serialMove(Serial2, legs[0].up, 350, 400);
	delay(10);
}

void intialPos() {
	for (int i = 3;i < 19;i += 3) {
		servo.serialMove(Serial2, i, initialUp, 600);
	}
	for (int i = 2;i < 18;i += 3) {
		servo.serialMove(Serial2, i, initialMid, 600);
	}
	for (int i = 1;i < 18; i += 3) {
		servo.serialMove(Serial2, i, initialDown, 600);
	}
}


int hexMove::getVin() {
	return servo.LobotSerialServoReadVin(Serial2, 1);
}


void hexMove::arrayInit(){
	commands[0] = {wave, waveHand};
	commands[1] = {forward , stepForward};
	commands[2] = {backwards, stepBackward};
	commands[3] = {right, turnRight};
	commands[4] = {left, turnLeft};
	commands[5] = {rightSideWalk, sideWalkRight};
	commands[6] = {leftSideWalk, sideWalkLeft};
	commands[7] = {inPose, intialPos};
	commands[8] = {stop, stopAll};

	pidCommands[0] = {lowX, pidCaseLowX};
	pidCommands[1] = {lowY, pidCaseLowY};
	pidCommands[2] = {highX, pidCaseHighX};
	pidCommands[3] = {highY, pidCaseHighY};

}