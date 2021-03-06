#pragma config(Sensor, in1,    stage2Encoder,  sensorPotentiometer)
#pragma config(Sensor, in2,    intakeEncoder,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  coneIntake,     sensorDigitalOut)
#pragma config(Sensor, dgtl2,  stage1Encoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl4,  highGoalLight,  sensorDigitalOut)
#pragma config(Motor,  port1,           rightDrive2,   tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port3,           leftDrive2,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port4,           stage1L,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           intakeR,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           intakeL,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           leftDrive1,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           rightDrive1,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           stage1R,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          stage2,        tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"
float desiredStage1 = 0;
float desiredStage2 = 0;
float stage2Limit = 0;
bool mobileGoal = true;
bool stage1Change = true;

int limit(int val, int min = -127, int max = 127){
	if(val>max){
		return max;
	}
	if(val<min){
		return min;
	}
	return val;
}

bool deadband(int val, int target, int db = 10){
	return val > target + db || val < -target - db;
}

void towerStage1(int val){
	motor[stage1L] = motor[stage1R] = val;
}

void towerStage2(int val){
	motor[stage2] = val;
}

void intake(int val){
	motor[intakeR] = motor[intakeL] = val;
}

void intakeCone(){
	SensorValue(coneIntake)=!SensorValue(coneIntake);
}

void drive(int rVal, int lVal){
	motor[rightDrive1] = motor[rightDrive2] = rVal;
	motor[leftDrive1] = motor[leftDrive2] = lVal;
}

void driveDeadband(int rVal, int lVal, int db = 10){
	if(deadband(rVal, 0, db))
		motor[rightDrive1] = motor[rightDrive2] = rVal;
	else
		motor[rightDrive1] = motor[rightDrive2] = 0;
	if(deadband(lVal, 0, db))
		motor[leftDrive1] = motor[leftDrive2] = lVal;
	else
		motor[leftDrive1] = motor[leftDrive2] = 0;
}

task driveControl(){
	while(true){
		driveDeadband(vexRT[Ch2], vexRT[Ch3], 10);
	}
}

void mobileGoalIntake(){
	intake(127);
	while(SensorValue(intakeEncoder) < 2000){
		delay(5);
	}
	intake(25);
	while(SensorValue(intakeEncoder) < 3350){
		delay(5);
	}
	intake(0);
	mobileGoal = true;
}

void mobileGoalOuttake(){
	intake(-127);
	while(SensorValue(intakeEncoder) > 2000){
		delay(5);
	}
	intake(-25);
	while(SensorValue(intakeEncoder) > 900){
		delay(5);
	}
	intake(0);
	mobileGoal = false;
}

task intakeControl(){
	bool accurate = false;
	while(true){
		if(vexRT[Btn7L] != accurate){
			accurate = !accurate;
			if(accurate){
				if(mobileGoal)
					mobileGoalOuttake();
				else
					mobileGoalIntake();
			}
		delay(5);
		}
	}
}

task stage1Control(){
	float old = SensorValue(stage1Encoder);
	desiredStage1 = SensorValue(stage1Encoder);
	float kP1 = 0.75;
	float kI1 = 0.0001;
	float kD1 = 0.02;
	float error = 0;
	float integral = 0;
	float derivative = 0;
	while(true){
		error = desiredStage1 - SensorValue(stage1Encoder);
		integral = integral + error;
		if(error == 0 || !stage1Change){
			integral = 0;
		}
		if(abs(error)>200){
			integral = 0;
		}
		if(desiredStage1 == SensorValue(stage1Encoder)){
			stage1Change = false;
		}
		derivative = error - old;
		old = SensorValue(stage1Encoder);
		towerStage1(limit(error * kP1 + integral * kI1 + derivative * kD1));
		delay(5);
	}
}

task stage2Control(){
	desiredStage2 = SensorValue(stage2Encoder);
	float kP2 = 0.25;
	float error = 0;
	while(true){
		error = desiredStage2 - SensorValue(stage2Encoder);
		towerStage2(limit(error * kP2));
		delay(5);
	}
}

void normalStackCone(int cone){
	if(cone == 1){
		desiredStage1 = -740;
		wait1Msec(500);
		desiredStage2 = 60 + stage2Limit;
	}
	else if(cone == 2){
		desiredStage1 = -780;
		wait1Msec(650);
		desiredStage2 = 130 + stage2Limit;
	}
	else if(cone == 3){
		desiredStage1 = -820;
		wait1Msec(600);
		desiredStage2 = 200 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 175 + stage2Limit;
	}
	else if(cone == 4){
		desiredStage1 = -835;
		wait1Msec(700);
		desiredStage2 = 400 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 250 + stage2Limit;
	}
	else if(cone == 5){
		desiredStage1 = -860;
		wait1Msec(800);
		desiredStage2 = 475 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 350 + stage2Limit;
	}
	else if(cone == 6){
		desiredStage1 = -875;
		wait1Msec(1200);
		desiredStage2 = 680 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 520 + stage2Limit;
	}
	else if(cone == 7){
		desiredStage1 = -865;
		wait1Msec(1050);
		desiredStage2 = 840 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 690 + stage2Limit;
	}
	else if(cone == 8){
		desiredStage1 = -840;
		desiredStage2 = 950 + stage2Limit;
		wait1Msec(1000);
		desiredStage2 = 1120 + stage2Limit;
		wait1Msec(400);
		desiredStage2 = 1000 + stage2Limit;
	}
	else if(cone == 9){
		desiredStage1 = -840;
		desiredStage2 = 1050 + stage2Limit;
		wait1Msec(950);
		desiredStage2 = 1260 + stage2Limit;
		wait1Msec(500);
		desiredStage2 = 1090 + stage2Limit;
	}
	else if(cone == 10){
		desiredStage1 = -825;
		desiredStage2 = 1100 + stage2Limit;
		wait1Msec(1000);
		desiredStage2 = 1355 + stage2Limit;
		wait1Msec(500);
		desiredStage2 = 1200 + stage2Limit;
	}
	else if(cone == 11){
		desiredStage1 = -790;
		desiredStage2 = 1250 + stage2Limit;
		wait1Msec(1050);
		desiredStage2 = 1600 + stage2Limit;
		wait1Msec(500);
		desiredStage2 = 1500 + stage2Limit;
	}
	else if(cone == 12){
		desiredStage1 = -755;
		desiredStage2 = 1400 + stage2Limit;
		wait1Msec(1100);
		desiredStage2 = 1880 + stage2Limit;
		wait1Msec(500);
		desiredStage2 = 1780 + stage2Limit;
	}
}

void highStackCone(int cone){
	if(cone == 1){
		desiredStage1 = 1;
		desiredStage2 = 1;
	}
	else if(cone == 2){
		desiredStage1 = 2;
		desiredStage2 = 2;
	}
	else if(cone == 3){
		desiredStage1 = 3;
		desiredStage2 = 3;
	}
	else if(cone == 4){
		desiredStage1 = 4;
		desiredStage2 = 4;
	}
	else if(cone == 5){
		desiredStage1 = 5;
		desiredStage2 = 5;
	}
	else if(cone == 6){
		desiredStage1 = 6;
		desiredStage2 = 6;
	}
	else if(cone == 7){
		desiredStage1 = 7;
		desiredStage2 = 7;
	}
	else{
		desiredStage1 = 13;
		desiredStage2 = 13;
	}
}

void groundPickUpCone(int stack){
	if(stack==7){
		desiredStage1 = -375;
		desiredStage2 = 1100 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else if(stack==8){
		desiredStage1 = -375;
		desiredStage2 = 1200 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else if(stack==9){
		desiredStage1 = -375;
		desiredStage2 = 1300 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else if(stack==10){
		desiredStage1 = -375;
		desiredStage2 = 1450 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else if(stack==11){
		desiredStage1 = -375;
		desiredStage2 = 1750 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else if(stack>=12){
		desiredStage1 = -375;
		desiredStage2 = 2060 + stage2Limit;
		wait1Msec(600);
		desiredStage2 = 800 + stage2Limit;
	}
	else{
		desiredStage1 = -375;
		desiredStage2 = 800 + stage2Limit;
	}
}

void lowGroundPickUpCone(){
	desiredStage1=0;
	desiredStage2=0;
}

void preloadPickUpCone(){
	desiredStage1=0;
	desiredStage2=0;
}

void setupLift(){
	desiredStage1 = -550;
	wait1Msec(900);
	stage2Limit = SensorValue(stage2Encoder);
	desiredStage2 = 800 + stage2Limit;
	wait1Msec(900);
	desiredStage1 = -375;
}

task coneControl(){
	int currentConeStack = 0;
	int currentHighConeStack = 0;
	bool tippedCone = false;
	bool intakeLowered = false;
	bool highGoal = false;
	while(true){
		if(!highGoal){
			SensorValue(highGoalLight)=0;
			if(vexRT[Btn7R]){
				currentConeStack = 0;
			}
			if(vexRT[Btn7U]){
				if(currentConeStack>1)
					currentConeStack--;
			}
			if(vexRT[Btn7D]){
				if(currentConeStack<13)
					currentConeStack++;
			}
			if(vexRT[Btn6U]){
				if(currentConeStack<13){
					currentConeStack++;
					normalStackCone(currentConeStack);
					stage1Change = true;
					intakeLowered = false;
				}
			}
		}
		if(highGoal){
			SensorValue(highGoalLight)=1;
			if(vexRT[Btn7R]){
				currentHighConeStack = 0;
			}
			if(vexRT[Btn7U]){
				if(currentHighConeStack>1)
					currentHighConeStack--;
			}
			if(vexRT[Btn7D]){
				if(currentHighConeStack<9)
					currentHighConeStack++;
			}
			if(vexRT[Btn6U]){
				if(currentHighConeStack<13){
					currentHighConeStack++;
					highStackCone(currentHighConeStack);
					stage1Change = true;
					intakeLowered = false;
				}
			}
		}
		if(vexRT[Btn6D] != tippedCone){
			tippedCone = !tippedCone;
			if(tippedCone){
				if(intakeLowered){
					groundPickUpCone(currentConeStack);
					stage1Change = true;
					intakeLowered = false;
				}
				else{
					lowGroundPickUpCone();
					stage1Change = true;
					intakeLowered = true;
				}
			}
		}
		if(vexRT[Btn5D] != highGoal){
			highGoal = !highGoal;
		}
	}
}

void pre_auton() {
	wait1Msec(100);
}

task autonomous {
	wait1Msec(100);
}

task usercontrol(){
	SensorValue[stage1Encoder]=0;
	startTask(intakeControl);
	startTask(coneControl);
	startTask(driveControl);
	startTask(stage1Control);
	startTask(stage2Control);
	setupLift();
	bool accurate = false;
	while(true){
		if(vexRT[Btn5U] != accurate){
			accurate = !accurate;
			if (accurate)
				intakeCone();
		}
	}
}
