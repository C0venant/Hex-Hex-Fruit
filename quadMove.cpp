#include "quadMove.h"
#include "busServo.h"



static busServo servo;

static Leg legs[6];

#define armDelta 40

static int16_t upperArmPosition = 700;
static int16_t midArmPosition = initialMid;
static bool stepTurn = true;

quadMove::quadMove(bool massage) {
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


static void changeH(uint8_t n, int16_t pos) {
	servo.serialMove(Serial2, legs[n-1].mid, pos, 200);
}

//changes gap between hands
void changeGap(){
	servo.serialMove(Serial2, legs[0].up, 1000-upperArmPosition, 500);
	servo.serialMove(Serial2, legs[5].up, upperArmPosition, 500);
}

void changeAngle(){
	servo.serialMove(Serial2, legs[0].mid, midArmPosition, 500);
	servo.serialMove(Serial2, legs[5].mid, midArmPosition, 500);
}

void spreadArms(){
	if(upperArmPosition<750){
		upperArmPosition+=armDelta;
	}
	changeGap();	
}

void narrowArms(){
	if(upperArmPosition>500){
		upperArmPosition-=armDelta;
	}
	changeGap();
}

void armsUp(){
	if(midArmPosition > 200){
		midArmPosition-=armDelta;
	}
	changeAngle();
}

void armsDown(){
	if(midArmPosition < 700){
		midArmPosition+=armDelta;
	}
	changeAngle();
}

//this section of code is for walking 
/*
#
         ---> tripod1
#    #


      #
        ---> tripod2 
#     #
*/

void strechLeg(uint8_t n, int sign){
	if(sign == 1){
		changeH(n, initialMid+100);
		if(n>3){
			servo.serialMove(Serial2, legs[n-1].up, initialUp+150, 200);
		}else{
			servo.serialMove(Serial2, legs[n-1].up, initialUp-150, 200);
		}
		
		servo.serialMove(Serial2, legs[n-1].down, initialDown+150, 200);
		changeH(n, initialMid);
	}else{
		servo.serialMove(Serial2, legs[n-1].down, initialDown, 200);
	}	
}

void tripod1Push(int sign){
	strechLeg(5, -1);
	servo.serialMove(Serial2, legs[1].up, initialUp, 200);
	servo.serialMove(Serial2, legs[2].down, initialDown+25, 200);
	servo.serialMove(Serial2, legs[3].up, initialUp, 200);
}

void tripod2Push(int sign){
	strechLeg(2, -1);
	servo.serialMove(Serial2, legs[2].up, initialUp, 200);
	servo.serialMove(Serial2, legs[3].down, initialDown+25, 200);
	servo.serialMove(Serial2, legs[4].up, initialUp, 200);
}

void backLegPosition(int n){
	changeH(n, initialMid - 100);
	if(n>3){
		servo.serialMove(Serial2, legs[n-1].up, initialUp+150, 200);
		
	}else{
		servo.serialMove(Serial2, legs[n-1].up, initialUp-150, 200);
	}
	servo.serialMove(Serial2, legs[n-1].down, initialDown, 200);
	delay(200);
	changeH(n, initialMid);
}


void step(){
	if(stepTurn){
		delay(300);
		strechLeg(5,1);
		delay(500);
		tripod1Push(1);
		delay(1000);
		backLegPosition(3);
		stepTurn = false;
	}else{
		delay(300);
		strechLeg(2,1);
		delay(500);
		tripod2Push(1);
		backLegPosition(4);
		stepTurn = true;
	}
}


void initialPos(){
	
	changeH(2, initialMid - 100);
	//changeH(4, initialMid - 100);
	servo.serialMove(Serial2, legs[1].up, initialUp-150, 400);
	//servo.serialMove(Serial2, legs[3].up, initialUp+150, 400);
	delay(300);
	changeH(2, initialMid);
	//changeH(4, initialMid);
	
	delay(500);
	servo.serialMove(Serial2, legs[0].down, 2000, 400);
	servo.serialMove(Serial2, legs[5].down, 2000, 400);
	servo.serialMove(Serial2, legs[0].up, initialUp-200, 400);
	servo.serialMove(Serial2, legs[5].up, initialUp+200, 400);
	backLegPosition(4);
}

void quadMove::arrayInit(){
	commands[0] = {quadInPose, initialPos};
	commands[1] = {spreadArm ,spreadArms};
	commands[2] = {narrowArm ,narrowArms};
	commands[3] = {changeAngleUp ,armsUp};
	commands[4] = {changeAngleDown ,armsDown};
	commands[5] = {quadForward, step};
}