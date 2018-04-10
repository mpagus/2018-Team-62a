#pragma config(Sensor, dgtl1,  stage1Encoder,  sensorQuadEncoder)
#pragma config(Motor,  port1,           stage1,        tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"
#include "math.h"

////////////////////////////////////////////////////
//    _____                       __  ___         //
//  |_   _|__  __ _ _ __ ___    / /_|___ \ __ _   //
//    | |/ _ \/ _` | '_ ` _ \  |  _ \ __) / _` |  //
//    | |  __/ (_| | | | | | | | (_) / __/ (_| |  //
//    |_|\___|\__,_|_| |_| |_|  \___/_____\__,_|  //
//                                                //
////////////////////////////////////////////////////

float batteryPower;

int followPath = 0;
bool completed = false;
bool outOfTheWay = false;
bool start = false;

void towerStage1(int val){
	motor[stage1] = val;
}

void resetLiftEncoders(){
	SensorValue(stage1Encoder) = 0;
}

float motionPIDGraphStage1(long time){
	if(followPath == 0){ //Unfold Robot
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 1){ //PickUpCone
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 2){ //SetUpCone
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 3){ //Cone1
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 4){ //Cone2
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 5){ //Cone3
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 6){ //Cone4
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 7){ //Cone5
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 8){ //Cone6
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 9){ //Cone7
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 10){ //Cone8
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 11 && outOfTheWay){ //Cone9 Only Out Of The Way
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0;
		}
	}
	return SensorValue(stage1Encoder);
}

float motionPIDGraphStage2(long time){
	if(followPath == 0){ //Unfold Robot
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 1){ //PickUpCone
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 2){ //SetUpCone
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 3){ //Cone1
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 4){ //Cone2
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 5){ //Cone3
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 6){ //Cone4
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 7){ //Cone5
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 8){ //Cone6
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 9){ //Cone7
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 10){ //Cone8
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else if(outOfTheWay){
			completed = true;
			return 0;
		}
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0*time;
		}
	}
	else if(followPath == 11 && outOfTheWay){ //Cone9 Only Out Of The Way
		if(time<1000)
			return 0*time;
		else if(time<1000)
			return 0*time;
		else{
			completed = true;
			return 0;
		}
	}
	return SensorValue(stage2Encoder);
}

void moveArm(int path, bool outOfWay = false){
	followPath = path;
	outOfTheWay = outOfWay;
	completed = false;
	while(!completed){
		wait1Msec(75);
	}
}

task motionPID(){
	int windowSize = 20;
	float kP1 = 1.2;
	float kI1 = 0.005;
	float kD1 = 4;
	float error1 = 0;
	float integral1 = 0;
	float derivative1 = 0;
	float old1 = 0;
	int windowSize = 0;
	long startTime = nPgmTime-10;
	long oldTime = nPgmTime-5;
	while(true){
		if(start){
			startTime = nPgmTime;
			start = false;
		}
		error1 = motionPIDGraph(nPgmTime-startTime+windowSize) - SensorValue(stage1Encoder);
		integral1 = integral1 + error1*(nPgmTime-oldTime);
		derivative1 = (error1 - old1)/(nPgmTime-oldTime);
		if(abs(error1)<1 || abs(error1)>100){
			integral1 = 0;
		}
		old1 = error1;
		towerStage1(limit(error1 * kP1 + integral1 * kI1 + derivative1 * kD1, -127, 127));
		oldTime = nPgmTime;
		delay(20);
	}
}

task dataLog(){
	datalogClear();
	datalogStart();
	while(true){
		datalogAddValueWithTimeStamp(0, motionPIDGraph(nPgmTime-startTime+windowSize));
		datalogAddValueWithTimeStamp(1, motionPIDGraph(nPgmTime-startTime));
		datalogAddValueWithTimeStamp(2, SensorValue(stage1Encoder));
		datalogAddValueWithTimeStamp(3, motor[stage1]);
		batteryPower = nImmediateBatteryLevel;
		wait1Msec(50);
	}
}

void pre_auton(){}

task autonomous(){}

task usercontrol(){
	resetLiftEncoders();
	startTask(dataLog);
	startTask(motionPID);
	while(true){
		if(vexRT[Btn8U]){
			followPath = 0;
			start=true;
			wait1Msec(500);
		}
		if(vexRT[Btn8R]){
			followPath = 1;
			start=true;
			wait1Msec(500);
		}
	}
}
