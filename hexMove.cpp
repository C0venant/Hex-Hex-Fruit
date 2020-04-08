#include "hexMove.h"
#include "busServo.h"

static bool stepTurn = true;
busServo servo;

hexMove::hexMove(bool massage) {}

struct Leg1 {
	uint8_t up = 6;
	uint8_t mid = 5;
	uint8_t down = 4;
};

struct Leg2 {
	uint8_t up = 15;
	uint8_t mid = 14;
	uint8_t down = 13;
};
struct Leg3 {
	uint8_t up = 18;
	uint8_t mid = 17;
	uint8_t down = 16;
};
struct Leg4 {
	uint8_t up = 12;
	uint8_t mid = 11;
	uint8_t down = 10;
};
struct Leg5 {
	uint8_t up = 3;
	uint8_t mid = 2;
	uint8_t down = 1;
};
struct Leg6 {
	uint8_t up = 9;
	uint8_t mid = 8;
	uint8_t down = 7;
};


static Leg1 leg1;
static Leg2 leg2;
static Leg3 leg3;
static Leg4 leg4;
static Leg5 leg5;
static Leg6 leg6;

static void liftUp(uint8_t n, int16_t pos) {
	if (n == 1) {
		servo.serialMove(Serial2, leg1.mid, pos, 200);
		delay(50);
	}
	if (n == 2) {
		servo.serialMove(Serial2, leg2.mid, pos, 200);
		delay(50);
	}
	if (n == 3) {
		servo.serialMove(Serial2, leg3.mid, pos, 200);
		delay(50);
	}
	if (n == 4) {
		servo.serialMove(Serial2, leg4.mid, pos, 200);
		delay(50);
	}
	if (n == 5) {
		servo.serialMove(Serial2, leg5.mid, pos, 200);
		delay(50);
	}
	if (n == 6) {
		servo.serialMove(Serial2, leg6.mid, pos, 200);
		delay(50);
	}
}
static void liftDown(uint8_t n, int16_t pos) {
	if (n == 1) {
		servo.serialMove(Serial2, leg1.mid, pos, 200);
		delay(50);
	}
	if (n == 2) {
		servo.serialMove(Serial2, leg2.mid, pos, 200);
		delay(50);
	}
	if (n == 3) {
		servo.serialMove(Serial2, leg3.mid, pos, 200);
		delay(50);
	}
	if (n == 4) {
		servo.serialMove(Serial2, leg4.mid, pos, 200);
		delay(50);
	}
	if (n == 5) {
		servo.serialMove(Serial2, leg5.mid, pos, 200);
		delay(50);
	}
	if (n == 6) {
		servo.serialMove(Serial2, leg6.mid, pos, 200);
		delay(50);
	}
}


// pushes body in horizontal direction
static void horPush(uint8_t n, int16_t pos) {
	if (n == 1) {
		servo.serialMove(Serial2, leg1.up, pos, 200);
		delay(50);
	}
	if (n == 2) {
		servo.serialMove(Serial2, leg2.up, pos, 200);
		delay(50);
	}
	if (n == 3) {
		servo.serialMove(Serial2, leg3.up, pos, 200);
		delay(50);
	}
	if (n == 4) {
		servo.serialMove(Serial2, leg4.up, pos, 200);
		delay(50);
	}
	if (n == 5) {
		servo.serialMove(Serial2, leg5.up, pos, 200);
		delay(50);

	}
	if (n == 6) {
		servo.serialMove(Serial2, leg6.up, pos, 200);
		delay(50);
	}
}

//change height of the body according to delta
void hexMove::changeH(int16_t delta, uint16_t v) {
		int down1 = servo.serialPos(Serial2, leg1.down);
		int mid1 = servo.serialPos(Serial2, leg1.mid);
		int down2 = servo.serialPos(Serial2, leg2.down);
		int mid2 = servo.serialPos(Serial2, leg2.mid);
		int down3 = servo.serialPos(Serial2, leg3.down);
		int mid3 = servo.serialPos(Serial2, leg3.mid);
		int down4 = servo.serialPos(Serial2, leg4.down);
		int mid4 = servo.serialPos(Serial2, leg4.mid);
		int down5 = servo.serialPos(Serial2, leg5.down);
		int mid5 = servo.serialPos(Serial2, leg5.mid);
		int down6 = servo.serialPos(Serial2, leg6.down);
		int mid6 = servo.serialPos(Serial2, leg6.mid);
		servo.serialMove(Serial2, leg1.mid, mid1 + delta, v);
		delay(50);
		servo.serialMove(Serial2, leg1.down, down1 + delta+20, v);
		delay(50);
		servo.serialMove(Serial2, leg2.mid, mid2 + delta, v);
		delay(50);
		servo.serialMove(Serial2, leg2.down, down2 + delta+20, v);
		delay(50);
		servo.serialMove(Serial2, leg3.mid, mid3 + delta, v);
		delay(50);
		servo.serialMove(Serial2, leg3.down, down3 + delta+20, v);
		delay(50);
		servo.serialMove(Serial2, leg4.mid, mid4 + delta, v);
		delay(50);
		servo.serialMove(Serial2, leg4.down, down4 + delta+20, v);
		delay(50);
		servo.serialMove(Serial2, leg5.mid, mid5 + delta, v);
		servo.serialMove(Serial2, leg5.down, down5 + delta+20, v);
		delay(50);
		servo.serialMove(Serial2, leg6.mid, mid6 + delta, v);
		delay(50);
		servo.serialMove(Serial2, leg6.down, down6 + delta+20, v);
		delay(50);
}

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

static void tripod1Up() {
	liftUp(1, 300);
	liftUp(3, 300);
	liftUp(5, 300);
}
static void tripod2Up() {
	liftUp(2, 300);
	liftUp(4, 300);
	liftUp(6, 300);
}
static void tripod1Down() {
	liftDown(1, 420);
	liftDown(3, 420);
	liftDown(5, 420);
}
static void tripod2Down() {
	liftDown(2, 420);
	liftDown(4, 420);
	liftDown(6, 420);
}
static void tripod1Push(int16_t pos) {
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 + pos);
}
static void tripod2Push(int16_t pos) {
	horPush(2, 500 - pos);
	horPush(4, 500 + pos);
	horPush(6, 500 + pos);
}
static void tripod1Turn(int16_t pos) {
	horPush(1, 500 - pos);
	horPush(3, 500 - pos);
	horPush(5, 500 - pos);
}
static void tripod2Turn(int16_t pos) {
	horPush(2, 500 - pos);
	horPush(4, 500 - pos);
	horPush(6, 500 - pos);
}

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
//tripod1 turn Rigth
static void turn1(int sign) {
	tripod1Up();
	tripod2Turn(0);
	tripod1Turn(sign * 150);
	tripod1Down();
	//stepCheck (1,3,5);   
}
static void turn2(int sign) {
	tripod2Up();
	tripod1Turn(0);
	tripod2Turn(sign * 150);
	tripod2Down();
	//stepCheck (2,4,6);      
}
// changes Height of legs for Pid
void hexMove::pidHeightControl(uint8_t n, int16_t midPos, int16_t lowPos, uint16_t v) {
	if (n == 1) {
		int down = servo.serialPos(Serial2, leg1.down);
		int mid = servo.serialPos(Serial2, leg1.mid);
		servo.serialMove(Serial2, leg1.mid, midPos, v);
		servo.serialMove(Serial2, leg1.down, lowPos, v);
	}
	if (n == 2) {
		int down = servo.serialPos(Serial2, leg2.down);
		int mid = servo.serialPos(Serial2, leg2.mid);
		servo.serialMove(Serial2, leg2.mid, midPos, v);
		servo.serialMove(Serial2, leg2.down, lowPos, v);
	}
	if (n == 3) {
		int down = servo.serialPos(Serial2, leg3.down);
		int mid = servo.serialPos(Serial2, leg3.mid);
		servo.serialMove(Serial2, leg3.mid, midPos, v);
		servo.serialMove(Serial2, leg3.down, lowPos, v);
	}
	if (n == 4) {
		int down = servo.serialPos(Serial2, leg4.down);
		int mid = servo.serialPos(Serial2, leg4.mid);
		servo.serialMove(Serial2, leg4.mid, midPos, v);
		servo.serialMove(Serial2, leg4.down, lowPos, v);
	}
	if (n == 5) {
		int down = servo.serialPos(Serial2, leg5.down);
		int mid = servo.serialPos(Serial2, leg5.mid);
		servo.serialMove(Serial2, leg5.mid, midPos, v);
		servo.serialMove(Serial2, leg5.down, lowPos, v);
	}
	if (n == 6) {
		int down = servo.serialPos(Serial2, leg6.down);
		int mid = servo.serialPos(Serial2, leg6.mid);
		servo.serialMove(Serial2, leg6.mid, midPos, v);
		servo.serialMove(Serial2, leg6.down, lowPos, v);
	}
}

void hexMove::stopAll() {
	for (uint8_t i = 1;i <= 18;i++) {
		servo.serialStop(Serial2, i);
	}
}

void hexMove::waveHand() {
	servo.serialMove(Serial2, leg1.mid, 200, 400);
	delay(100);
	servo.serialMove(Serial2, leg1.down, 400, 400);
	delay(300);
	servo.serialMove(Serial2, leg1.up, 600, 400);
	delay(600);
	servo.serialMove(Serial2, leg1.up, 350, 400);
	delay(10);
}

void hexMove::intialPos(int16_t pos3, int16_t pos2, int16_t pos1) {
	for (int i = 3;i < 19;i += 3) {
		servo.serialMove(Serial2, i, pos3, 600);
		delay(50);
	}
	for (int i = 2;i < 18;i += 3) {
		servo.serialMove(Serial2, i, pos2, 600);
		delay(50);
	}
	for (int i = 1;i < 18; i += 3) {
		servo.serialMove(Serial2, i, pos1, 600);
		delay(50);
	}
}

void hexMove::walking(uint8_t blue) {
	if (forward == blue) {
		if (stepTurn) {
			stepTurn = false;
			step1(1);
		}
		else {
			stepTurn = true;
			step2(1);
		}
	}
	else if (right == blue) {
		if (stepTurn) {
			stepTurn = false;
			turn1(1);
		}
		else {
			stepTurn = true;
			turn2(1);
		}
	}
	else if (left == blue) {
		if (stepTurn) {
			stepTurn = false;
			turn1(-1);
		}
		else {
			stepTurn = true;
			turn2(-1);
		}
	}
	else if (backwards == blue) {
		if (stepTurn) {
			stepTurn = false;
			step1(-1);
		}
		else {
			stepTurn = true;
			step2(-1);
		}
	}
	else if (inPose == blue) {
		intialPos(500, 420, 70);
	}
}

void hexMove::processCommand(BLUE blue) {
	if (blue.command >= 49 && blue.command <= 54) {
		walking(blue.command);
	}
	else if (wave == blue.command) {
		waveHand();
	}
	else if (changeHeight == blue.command) {
		changeH(blue.value, 600);
	}
	
}

int hexMove::getVin() {
	return servo.LobotSerialServoReadVin(Serial2, 1);
}