#pragma config(Sensor, dgtl1,  stage1Encoder,  sensorQuadEncoder)
#pragma config(Motor,  port1,           stage1,        tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma platform(VEX2)
#pragma competitionControl(Competition)
#include "Vex_Competition_Includes.c"
#include "MilesLib.h"

////////////////////////////////////////////////////
//    _____                       __  ___         //
//  |_   _|__  __ _ _ __ ___    / /_|___ \ __ _   //
//    | |/ _ \/ _` | '_ ` _ \  |  _ \ __) / _` |  //
//    | |  __/ (_| | | | | | | | (_) / __/ (_| |  //
//    |_|\___|\__,_|_| |_| |_|  \___/_____\__,_|  //
//                                                //
////////////////////////////////////////////////////

float batteryPower;
float desiredStage1 = 0;
float desiredStage2 = 0;

void towerStage1(int val){
	motor[stage1] = val;
}

void resetLiftEncoders(){
	SensorValue(stage1Encoder) = 0;
}

int followPath = 0;

float motionPIDGraph(long time){
	if(followPath==0){
		if(time<1000)
			return time^2/600 + 1.7*time;
	}
	else if(followPath==1){
		if(time<1000)
			return time^2/600 + 1.7*time;
	}
}

int windowSize = 0;
float kP_p1 = 2.6;
float kI_p1 = 0.02;
float kD_p1 = 10;
float kVFF_p1 = 0;
float kP_v1 = 2.6;
float kD_v1 = 10;
long startTime = nPgmTime;

bool start = false;
task motionPID(){
	float old = SensorValue(stage1Encoder);
	float error_p1 = 0;
	float integral_p1 = 0;
	float derivative_p1 = 0;
	float old_p1 = 0;
	int windowSize = 0;
	float expectedVelocity = 0;
	float velocity = 0;
	float error_v1 = 0;
	float derivative_v1 = 0;
	float drive_v1 = 0;
	long oldTime = nPgmTime;
	long startTime = nPgmTime;
	while(true){
		velocity = (error_p1 - old_p1)/(nPgmTime-oldTime);
		if(start){
			startTime = nPgmTime;
			start = false;
		}
		error_p1 = motionPIDGraph(nPgmTime-startTime+windowSize) - SensorValue(stage1Encoder);
		integral_p1 = integral_p1 + error_p1*(nPgmTime-oldTime);
		derivative_p1 = (error_p1 - old_p1)/(nPgmTime-oldTime);
		if(deadband(error_p1, 0, 5) || abs(error_p1)>40){
			integral_p1 = 0;
		}
		old_p1 = error_p1;
		expectedVelocity = limit(error_p1 * kP_p1 + integral_p1 * kI_p1 + derivative_p1 * kD_p1 + kVFF_p1 * velocity, -127, 127);
		error_v1 = motionPIDGraph(nPgmTime-startTime) - SensorValue(stage1Encoder);
		derivative_v1 = (error_v1 - old_v1)/(nPgmTime-oldTime);
		old_v1 = error_v1;
		drive_v1 = drive_v1 + error_p1 * kP_v1 + integral_v1 * kI_v1 + derivative_p1 * kD_v1;
		towerStage1(drive_v1);
		oldTime = nPgmTime;
		delay(20);
	}
}

task dataLog(){
	datalogClear();
	datalogStart();
	while(true){
		datalogAddValueWithTimeStamp(0, motionPIDGraph(nPgmTime-startTime+windowSize));
		datalogAddValueWithTimeStamp(1, SensorValue(stage1Encoder));
		datalogAddValueWithTimeStamp(2, motor[stage1]);
		batteryPower = nImmediateBatteryLevel;
		wait1Msec(10);
	}
}

task autonomous(){}

task usercontrol(){
	resetLiftEncoders();
	startTask(dataLog);
	startTask(coneControl);
	startTask(driveControl);
	startTask(mobileGoalMotors);
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