#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(UART_Usage, UART2, uartNotUsed, baudRate4800, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    PowerExpander,  sensorAnalog)
#pragma config(Sensor, in2,    Gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  leftEncoder,    sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  coneButtonSensor, sensorTouch)
#pragma config(Sensor, dgtl5,  stage2Encoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl8,  stage1Encoder,  sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign )
#pragma config(Motor,  port1,           stage1,        tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           intakeR,       tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           intakeL,       tmotorVex393HighSpeed_MC29, openLoop, reversed, encoderPort, I2C_1)
#pragma config(Motor,  port4,           rightDrive2,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           leftDrive2,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           leftDrive1,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port7,           rightDrive1,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           rightDrive3,   tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           leftDrive3,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port10,          stage2,        tmotorVex393HighSpeed_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Miles_Vex_Competition_Includes.c"

#include "MilesLib.h"

////////////////////////////////////////////////////
//    _____                       __  ___         //
//  |_   _|__  __ _ _ __ ___    / /_|___ \ __ _   //
//    | |/ _ \/ _` | '_ ` _ \  |  _ \ __) / _` |  //
//    | |  __/ (_| | | | | | | | (_) / __/ (_| |  //
//    |_|\___|\__,_|_| |_| |_|  \___/_____\__,_|  //
//                                                //
////////////////////////////////////////////////////

//Note to Cameron: please start by button testing the program and inputting the sensors
//Next start tuning the mobile goal intake states and the values for each
//Next start reading values for normal stacking cones
//Then add the pickup locations and stuff
//Then add preload capability
//Then add high goal shit

float batteryPower; //battery power uopdated in DataLog task always running in the background
bool mobileGoal = true;
float desiredStage1 = 0;
float desiredStage2 = 0;
float stage2Scalar = 599;
int currentConeStack = 0;
int currentHighConeStack = 0;
bool autonRan = false;
bool mobileTip = false;
bool mobileUntip = false;
bool mobileGoalTip = false;
bool intakeLowered = true;
bool letIntakeGo = false;
int autonNumber = 0;
float LCDVar1 = 0;
float LCDVar2 = 0;
bool autonRunning = false;

//Used to assign motor powers to intake
void intake(int val){
	motor[intakeL] = motor[intakeR] = val;
}

//Used to assign motor powers to intake (seperate sides)
void intakeSides(int rval, int lval){
	motor[intakeL] = lval;
	motor[intakeR] = rval;
}

//Used to assign motor powers to drive
void drive(int rVal, int lVal){
	motor[rightDrive1] = motor[rightDrive2] = motor[rightDrive3] = rVal;
	motor[leftDrive1] = motor[leftDrive2] = motor[leftDrive3] = lVal;
}

//Used to assign motor powers to stage 1
void towerStage1(int val){
	motor[stage1] = val;
}

//Used to assign motor powers to stage 2
void towerStage2(int val){
	motor[stage2] = val;
}

//Reset lift encoders
void resetLiftEncoders(){
	SensorValue(stage1Encoder) = 0;
	SensorValue(stage2Encoder) = 0;
}

float kP1 = 1.60;
float kI1 = 0.005;
float kD1 = 140;

//Stage 1 pid control
task stage1Control(){
	desiredStage1 = SensorValue(stage1Encoder);
	float old = SensorValue(stage1Encoder);
	float error = 0;
	float integral = 0;
	float derivative = 0;
	long oldTime = 0;
	while(true){
		error = desiredStage1 - SensorValue(stage1Encoder);
		integral = integral + (error*(nPgmTime-oldTime));
		derivative = (error - old)/(nPgmTime-oldTime);
		if(abs(error)<3 || abs(error)>100){
			integral = 0;
		}
		oldTime = nPgmTime;
		old = error;
		towerStage1(limit(error * kP1 + integral * kI1 + derivative * kD1, -100, 100));
		delay(25);
	}
}

int numRevolutions = 0;
int ticksPerRevolution = 1800; //Make sure this is correct. It is used for revolutions

float kP2 = 0.35;
float kI2 = 0.003;
float kD2 = 4;
//Stage 2 pid control
task stage2Control(){
	desiredStage2 = (SensorValue(stage2Encoder) - stage2Scalar);
	float old = SensorValue(stage2Encoder);
	float error = 0;
	float integral = 0;
	float derivative = 0;
	long oldTime = nPgmTime-10;
	while(true){
		error = (desiredStage2+numRevolutions*ticksPerRevolution) - (SensorValue(stage2Encoder) - stage2Scalar);
		integral = integral + (error*(nPgmTime-oldTime));
		derivative = (error - old)/(nPgmTime-oldTime);
		if(abs(error)<10 || abs(error)>80){
			integral = 0;
		}
		oldTime = nPgmTime;
		old = error;
		towerStage2(limit(error*kP2 + derivative*kD2 + integral*kI2, -100, 100));
		delay(20);
	}
}

////This task runs in the background to reset the pid (resets desired to current value) and halt motor power (ideally)
////It needs to be completely tuned
//task stallCheck(){
//	float oldEncoderStage1 = SensorValue(stage1Encoder);
//	float oldEncoderStage2 = SensorValue(stage2Encoder);
//	int stage1Tick = 0;
//	int stage2Tick = 0;
//	while (true){
//		oldEncoderStage1 = SensorValue(stage1Encoder);
//		oldEncoderStage2 = SensorValue(stage2Encoder);
//		if(stage1Tick>30){ //waits for 30*75 milliseconds of ticks to reset stage 1
//			stopTask(stage1Contol);
//			wait1Msec(100);
//			startTask(stage1Control);
//		}
//		if(stage2Tick>30){ //waits for 30*75 milliseconds of ticks to reset stage 2
//			stopTask(stage2Contol);
//			wait1Msec(100);
//			startTask(stage2Control);
//		}
//		if(abs(SensorValue(stage1Encoder) - oldEncoderStage1)>25 && abs(desiredStage1 - SensorValue(stage1Encoder))>20) //What causes stage 1 to tick
//			stage1Tick++;
//		else
//			stage1Tick = 0;
//		if(abs(SensorValue(stage2Encoder) - oldEncoderStage2)>25 && abs(desiredStage2+numRevolutions*ticksPerRevolution) - (SensorValue(stage2Encoder) - stage2Scalar)>20) //What causes stage 2 to tick
//			stage2Tick++;
//		else
//			stage2Tick = 0;
//		wait1Msec(75);
//	}
//}

//Moves stage 1 to a desired value and cuts out when it hits the contiunue value
//The first number is the error continue
void moveStage1WaitUntil(float desiredValue, float continueValue){
	desiredStage1 = desiredValue;
	moveSingleStageWaitUntil(stage1Encoder, continueValue, 20);
}

//Moves stage 1 to desired values and waits for completion
//The first number is the error continue
//The second number is the derivative continue
//The third number is the amount of 20 second clicks to move on
void moveStage1Wait(float desiredValue, int db=20){
	desiredStage1 = desiredValue;
	moveSingleStageWait(stage1Encoder, desiredValue, db, 5, 3);
}

//Moves stage 2 to a desired value and cuts out when it hits the contiunue value
//The first number is the error continue
void moveStage2WaitUntil(float desiredValue, float continueValue){
	desiredStage2 = desiredValue;
	//moveSingleStageWaitUntil(stage2Encoder, continueValue, 50);
	moveSingleStageWaitUntil(stage2Encoder, continueValue+numRevolutions*ticksPerRevolution + stage2Scalar, 20);
}

//Moves stage 2 to desired values and waits for completion
//The first number is the error continue
//The second number is the derivative continue
//The third number is the amount of 20 second clicks to move on
void moveStage2Wait(float desiredValue){
	desiredStage2 = desiredValue;
	//moveSingleStageWait(stage2Encoder, desiredValue, 50, 50);
	moveSingleStageWait(stage2Encoder, desiredValue+numRevolutions*ticksPerRevolution + stage2Scalar, 20, 5, 3);
}

//Moves both stages to desired values and waits for completion
//The first number is the error continue
//The second number is the derivative continue
//The final number is the amount of 20 second clicks to move on
void moveBothStagesWait(float desiredValue1, float desiredValue2){
	desiredStage1 = desiredValue1;
	desiredStage2 = desiredValue2;
	//moveDoubleStageWait(stage1Encoder, desiredValue1, 60, 60, stage2Encoder, desiredValue2,  60, 60);
	moveDoubleStageWait(stage1Encoder, desiredValue1, 20, 5, stage2Encoder, desiredValue2+numRevolutions*ticksPerRevolution + stage2Scalar, 20, 5, 3);
}

//Makes a revolution of stage 2
void stage2RevolutionNoWait(){
	numRevolutions = numRevolutions+1;
}

//Makes a revolution of stage 2 (waits for it to be completed)
void stage2Revolution(){
	numRevolutions = numRevolutions+1;
	moveSingleStageWait(stage1Encoder, desiredStage2+numRevolutions*ticksPerRevolution - stage2Scalar, 20, 20, 6);
}

//Picks up normal cone (with jab)
void groundPickUpCone(){
	moveStage1Wait(-15, 35);
	wait1Msec(20);
}

//Picks up normal cone (waits to reach value)
void groundPickUpConeWait(){
	moveStage1Wait(-15, 35);
	wait1Msec(120);
	desiredStage1 = SensorValue(stage1Encoder)+7;
	desiredStage2 = SensorValue(stage2Encoder)-stage2Scalar+10;
}

//Hovers above normal cone
void groundSetUpCone(){
	desiredStage1 = 190;
	desiredStage2 = 40;
}

//Hovers above normal cone (waits to reach value)
void groundSetUpConeWait(){
	moveBothStagesWait(190, 40);
}

//Picks up preload
void preloadPickUpCone(){
	desiredStage1 = 185;
	desiredStage2 = 25;
}

//Hovers above preload
void preloadSetUpCone(){
	desiredStage1 = 185;
	desiredStage2 = 25;
}

//This is for normally stacking cones. The preload variable isn't currently used, but it could be used to change the pathway if it needs to due to the wall
//General Form:
//1: Start moving stage 1 to its position
//2: ASAP move stage 2 and stage 1 to be hovering above the cone stack
//3: Lower the cone to below its location
//4: Move Stage 1 down as you return to detach from the stack
//Obviously, there are slight variations, but nothing too much
//cone
void normalStackCone(int cone, bool preload = false){
	//cone1
	if(cone == 1){
		//moveStage1WaitUntil(405, 345);
		//moveBothStagesWait(405, -270);
		//moveBothStagesWait(190, -350);
		moveStage1WaitUntil(380, 280);
		desiredStage2 = -400;
		moveStage1Wait(380);
		moveBothStagesWait(155, -400);
	}
	//cone2
	else if(cone == 2){
		//moveStage1WaitUntil(520, 435);
		//moveBothStagesWait(520, -200);
		//moveBothStagesWait(285, -310);
		moveStage1WaitUntil(470, 400);
		desiredStage2 = -365;
		moveStage1Wait(470);
		moveBothStagesWait(250, -365);
	}
	//cone3
	else if(cone == 3){
		//moveStage1WaitUntil(640, 505);
		//desiredStage1 = 695;
		//wait1Msec(50);
		//moveStage2Wait(-10);
		//moveBothStagesWait(400, -200);
		moveStage1WaitUntil(690, 630);
		desiredStage2 = -260;
		moveBothStagesWait(365, -260);
	}
	//cone4
	else if(cone == 4){
		moveStage1WaitUntil(265, 150);
		moveBothStagesWait(260, 1250);
		desiredStage1 = 0;
		wait1Msec(190);
		groundSetUpCone();
		desiredStage1 = 50;
		wait1Msec(200);
	}
	//cone5
	else if(cone == 5){
		moveStage1WaitUntil(330, 130);
		moveBothStagesWait(325, 1235);
		desiredStage1 = 80;
		wait1Msec(190);
		groundSetUpCone();
		desiredStage1 = 130;
		wait1Msec(200);
	}
	//cone6
	else if(cone == 6){
		moveStage1WaitUntil(370, 130);
		moveBothStagesWait(365, 1175);
		desiredStage1 = 140;
		wait1Msec(190);
		groundSetUpCone();
		desiredStage1 = 190;
		wait1Msec(200);
	}
	//cone7
	else if(cone == 7){
		moveStage1WaitUntil(450, 130);
		moveBothStagesWait(445, 1120);
		desiredStage1 = 215;
		wait1Msec(150);
		groundSetUpCone();
		desiredStage1 = 265;
		wait1Msec(100);
	}
	//cone8
	else if(cone == 8){
		moveStage1WaitUntil(615, 300);
		moveBothStagesWait(615, 935);
		moveBothStagesWait(445, 1058);
		desiredStage1 = 300;
		wait1Msec(100);
		groundSetUpCone();
		desiredStage1 = 350;
		wait1Msec(100);
	}
}

//This just goes directly to a position that hovers right above where the cone needs to be stacked
//Each cone is simply given this position
//coneH
void highStackCone(float cone){
	//cone1H
	if(cone == 1){
		moveBothStagesWait(145, -195);
	}
	//cone2H
	else if(cone == 2){
		moveBothStagesWait(215, -215);
	}
	//cone3H
	else if(cone == 3){
		moveBothStagesWait(235, -235);
	}
	//cone4H
	else if(cone == 4){
		moveBothStagesWait(287, 755);
	}
	//cone5H
	else if(cone == 5){
		moveBothStagesWait(355, 755);
	}
}

//This goes from hovering above to stacking the cone, waiting and moving it to a location to go back to normal
//Each cone just has a specific place to go from its hovering location it its stacked location
//Then it all moves to the final detached location
//coneD
void highStackDetach(int cone){
	//cone1D
	if(cone == 1){
		moveBothStagesWait(145, -195);
	}
	//cone2D
	else if(cone == 2){
		moveBothStagesWait(215, -215);
	}
	//cone3D
	else if(cone == 3){
		moveBothStagesWait(235, -235);
	}
	//cone4D
	else if(cone == 4){
		moveBothStagesWait(287, 755);
	}
	//cone5D
	else if(cone == 5){
		moveBothStagesWait(355, 755);
	}
	wait1Msec(350);
	moveBothStagesWait(250, 270); //Final detached location
}

//Low get out of the way for 5+ cones
void getOutOfTheWay(){
	desiredStage1 = 610;
	desiredStage2 = 980;
}

//Low get out of the way for 1-4 cones
void getOutOfTheWayMid(){
	desiredStage1 = 360;
	desiredStage2 = 1300;
}

//Low get out of the way for no cones (only in auton)
void getOutOfTheWayLow(int until = 110){
	if(SensorValue(stage1Encoder)>170)
		desiredStage2 = -450;
	else{
		moveStage1WaitUntil(170, until);
		desiredStage2 = -450;
	}
}

//Unfolds the robot for driver control
void unfoldRobot(){
	moveStage1Wait(220);
	moveStage2Wait(0);
	groundSetUpConeWait();
	//moveStage1WaitUntil(482, 450);
	//mobileGoal = false;
}

//Unfolds the robot during atuonomous
void unfoldRobotAuton(){
	moveStage1Wait(220);
	getOutOfTheWayLow();
}

task coneControl(){
	currentConeStack = 0;
	currentHighConeStack = 0;
	bool highGoalDetach = false;
	bool pickUpCone = false;
	bool messUpButton = false;
	bool incButton = false;
	bool coneButton = false;
	intakeLowered = true;
	bool outHigh = false;
	bool aAccurate = false;
	bool bAccurate = false;
	while(true){
		if(!vexRT[Btn7U] && !vexRT[Btn7L]){ //Normal mode
			if(vexRT[Btn8R]){ //Reset cone height
				currentConeStack = 0;
			}
			if(vexRT[Btn8D] != messUpButton){ //Decrease cone height
				messUpButton = !messUpButton;
				if(messUpButton && currentConeStack > 0){
					currentConeStack--;
				}
			}
			if(vexRT[Btn8U] != incButton){ //Increase cone height
				incButton = !incButton;
				if(incButton && currentConeStack < 7){
					currentConeStack++;
				}
			}
			if(vexRT[Btn6U] != pickUpCone){ //Pick up cone
				pickUpCone = !pickUpCone;
				if(pickUpCone){
					int timer = 0;
					while(vexRT[Btn6U] && timer<20){
						timer++;
						wait1Msec(10);
					}
					if(!vexRT[Btn6U] && currentConeStack < 8){
						currentConeStack++;
						normalStackCone(currentConeStack);
						groundSetUpCone();
						intakeLowered = true;
					}
					while(vexRT[Btn6U]){
						if(SensorValue(coneButtonSensor)){
							wait1Msec(30);
							groundPickUpCone();
							wait1Msec(50);
							currentConeStack++;
							normalStackCone(currentConeStack);
							groundSetUpCone();
							intakeLowered = true;
						}
						if(vexRT[Btn8R]){ //Reset cone height
							currentConeStack = 0;
						}
						if(vexRT[Btn8D] != messUpButton){ //Decrease cone height
							messUpButton = !messUpButton;
							if(messUpButton && currentConeStack > 0){
								currentConeStack--;
							}
						}
						if(vexRT[Btn8U] != incButton){ //Increase cone height
							incButton = !incButton;
							if(incButton && currentConeStack < 7){
								currentConeStack++;
							}
						}
						wait1Msec(50);
					}
				}
			}
		}
		/**if(vexRT[Btn7U]){ //Preload mode
		if(vexRT[Btn8R]){ //Reset cone height
		currentConeStack = 0;
		}
		if(vexRT[Btn8D] != messUpButton){ //Decrease cone height
		messUpButton = !messUpButton;
		if(messUpButton && currentConeStack > 0){
		currentConeStack--;
		}
		}
		if(vexRT[Btn8U] != incButton){ //Increase cone height
		incButton = !incButton;
		if(incButton && currentConeStack < 7){
		currentConeStack++;
		}
		}
		if(vexRT[Btn6U] != pickUpCone){ //Pick up cone
		pickUpCone = !pickUpCone;
		if(pickUpCone){
		if(currentConeStack < 8){
		currentConeStack++;
		normalStackCone(currentConeStack, true);
		preloadSetUpCone();
		intakeLowered = true;
		}
		}
		}
		}
		else{ //High goal mode
		if(vexRT[Btn8R]){ //Reset cone height
		currentHighConeStack = 0;
		}
		if(vexRT[Btn8D] != messUpButton){ //Decrease cone height
		messUpButton = !messUpButton;
		if(messUpButton && currentHighConeStack > 0){
		currentHighConeStack--;
		}
		}
		if(vexRT[Btn8U] != incButton){ //Increase cone height
		incButton = !incButton;
		if(incButton && currentHighConeStack < 7){
		currentHighConeStack++;
		}
		}
		if(vexRT[Btn6U] != pickUpCone){ //Toggle BEtween above stack height on high goal and dropping the cone
		pickUpCone = !pickUpCone;
		if(pickUpCone){
		if(highGoalDetach){
		highGoalDetach = false;
		highStackDetach(currentHighConeStack);
		}
		else if(currentHighConeStack < 7){
		currentHighConeStack++;
		highStackCone(currentHighConeStack);
		highGoalDetach = true;
		intakeLowered = false;
		}
		}
		}
		}*/
		if(vexRT[Btn6D] != coneButton){ //Toggle between above cone and at cone height for pickup
			coneButton = !coneButton;
			if(coneButton){
				if(intakeLowered){
					groundPickUpConeWait();
					intakeLowered = false;
				}
				else{
					groundSetUpConeWait();
					intakeLowered = true;
				}
			}
		}
		if(vexRT[Btn7R] != outHigh){ //Get out of the way high
			outHigh = !outHigh;
			if(outHigh){
				getOutOfTheWay();
				intakeLowered = false;
			}
		}
		if(vexRT[Btn5U] != aAccurate){ //creates the lock to pause this task while the mobile goal moves (no multi-threading errors)
			aAccurate = !aAccurate;
			if(aAccurate){
				letIntakeGo = true;
				while(letIntakeGo){
					delay(50);
				}
			}
		}
		if(vexRT[Btn5D] != bAccurate){
			bAccurate = !bAccurate;
			if(bAccurate){
				letIntakeGo = true;
				while(letIntakeGo){
					delay(50);
				}
			}
		}
		delay(5);
	}
}

//The mobile goal task for user control.
task mobileGoalMotors(){
	bool accurate = false;
	bool baccurate = false;
	bool caccurate = false;
	mobileGoal = true;
	int outCount = 0;
	int inCount = 116;
	int integral = 0;
	while(true){
		//Tipping mode
		if(mobileTip){
			if(mobileGoalTip){
				integral+=320-nMotorEncoder[intakeL];
				intake(0.85*(320-nMotorEncoder[intakeL])+0.001*integral); //general pid to get it to the pre-tip value
			}
			else{
				if(nMotorEncoder[intakeL]<715){
					intakeSides(35, 50); //brings the intake down unevenly
				}
				else{
					intake(10);
				}
			}
		}
		//Untipping mode
		else if(mobileUntip){
			integral+=350-nMotorEncoder[intakeL];
			intake(0.85*(350-nMotorEncoder[intakeL])+0.001*integral); //general pid to keep it at the untip value
		}
		//Mobile goal out
		else if(!mobileGoal){
			if(currentConeStack<3){
				if(outCount<110){
					intake(127);
					outCount++;
				}
				else if(outCount<140){
					intake(60);
					outCount++;
				}
				else{
					if(vexRT[Ch2] > 5 || vexRT[Ch3] > 5)
						intake(19);
					else
						intake(0);
				}
			}
			else{
				if(outCount<80){
					intake(127);
					outCount++;
				}
				else if(outCount<140){
					intake(40);
					outCount++;
				}
				else{
					if(vexRT[Ch2] > 5 || vexRT[Ch3] > 5)
						intake(19);
					else
						intake(0);
				}
			}
		}
		//Mobile goal in
		else{
			if(inCount<120){
				intake(-127);
				inCount++;
			}
			else if(inCount<135){
				intake(-7);
				inCount++;
			}
			else
				intake(-2);
		}
		//Out/in control
		if(vexRT[Btn5U] != accurate){
			accurate = !accurate;
			if(accurate){
				while(!letIntakeGo){
					wait1Msec(15); //This is essentially a lock that waits for the cone control method to pause for the mobile goal to move (it pritects against multi-threading errors)
				}
				mobileGoal = !mobileGoal;
				inCount = 0;
				outCount = 0;
				mobileTip = false;
				mobileGoalTip = false;
				if(!mobileGoal){
					//This moves the arm out of the way to different locations based on the cone stack (If the arm moves out)
					if(currentConeStack<=0){
						getOutOfTheWayLow(80);
						wait1Msec(40);
						intakeLowered = false;
					}
					else if(currentConeStack<=4){
						getOutOfTheWayMid();
						wait1Msec(80+40*currentConeStack);
						intakeLowered = false;
					}
					else{
						getOutOfTheWay();
						wait1Msec(80 + 40*currentConeStack);
						intakeLowered = false;
					}
					currentConeStack = 0;
				}
				letIntakeGo = false;
			}
		}
		//Tipping control
		if(vexRT[Btn5D] != baccurate){
			baccurate = !baccurate;
			if(baccurate){
				while(!letIntakeGo){
					wait1Msec(15); //This is essentially a lock that waits for the cone control method to pause for the mobile goal to move (it pritects against multi-threading errors)
				}
				getOutOfTheWayLow();
				integral = 0;
				mobileTip = true;
				mobileGoalTip = !mobileGoalTip;
				mobileGoal = false;
				mobileUntip = false;
				letIntakeGo = false;
			}
		}
		//Untipping control
		if(vexRT[Btn7D] != caccurate){
			caccurate = !caccurate;
			if(caccurate){
				while(!letIntakeGo){
					wait1Msec(15); //This is essentially a lock that waits for the cone control method to pause for the mobile goal to move (it pritects against multi-threading errors)
				}
				getOutOfTheWayLow();
				integral = 0;
				mobileGoal = false;
				mobileUntip = true;
				mobileTip = false;
				letIntakeGo = false;
			}
		}
		delay(5);
	}
}

//Auton mobile goal task runs in the background without control except the mobileGoal boolean.
//The mobile goal task for user control.
task mobileGoalAuton(){
	mobileGoal = true;
	int outCount = 0;
	int inCount = 0;
	while(true){
		//Mobile goal out
		if(!mobileGoal){
			if(outCount<80){
				intake(127);
				outCount++;
			}
			else if(outCount<140){
				intake(40);
				outCount++;
			}
			else{
				intake(10);
			}
		}
		//Mobile goal in
		else{
			if(inCount<102){
				intake(-127);
				inCount++;
			}
			else if(inCount<115){
				intake(-7);
				inCount++;
			}
			else
				intake(-2);
		}
		delay(5);
	}
}

void mobileGoalIn(){
	mobileGoal = true;
	wait1Msec(620);
}

void mobileGoalOut(){
	mobileGoal = true;
	wait1Msec(680);
}

//This task constantly runs in the background. Change the values as needed to get that data!
task dataLog(){
	datalogClear();
	datalogStart();
	while(true){
		datalogAddValueWithTimeStamp(0, desiredStage1);
		datalogAddValueWithTimeStamp(1, SensorValue(stage1Encoder));
		datalogAddValueWithTimeStamp(2, motor[stage1]);
		datalogAddValueWithTimeStamp(3, desiredStage2);
		datalogAddValueWithTimeStamp(4, SensorValue(stage2Encoder));
		datalogAddValueWithTimeStamp(5, motor[stage2]);
		datalogAddValueWithTimeStamp(6, -desiredDrive);
		datalogAddValueWithTimeStamp(7, desiredTurn);
		batteryPower = nImmediateBatteryLevel;
		wait1Msec(75);
	}
}

//This refers to the autonomus stuff. I would reccommend doing nothing to this file. It gives all the functions for autonomous.
#include "Team 62 Mark VI Match.c"
#include "Team 62 Mark VI LCD.c"

//This runs at the beginning of each reboot and calibrates the gyro. Keep the robot still for 2 seconds to calibrate.
void pre_auton() {
	calibrateGyros();
	startTask(LCDTask);
}

//This takes from Match for get the auton function which takes one of the possible autons.
//The testPID() is the only test you need that goes back and forth to test straight and turning pid.
task autonomous(){
	startTask(dataLog);
	startTask(LCDTask);
	auton(autonNumber);
	//testPID();
	//testTurn();
}

//User control only deploys when autonRan is false
task usercontrol(){
	startTask(dataLog);
	startTask(coneControl);
	startTask(driveControl);
	startTask(mobileGoalMotors);
  startTask(LCDTask);
	resetEncoders();
	if(!autonRan){
		resetLiftEncoders();
		startTask(stage1Control);
		startTask(stage2Control);
		//startTask(stallCheck);
		unfoldRobot();
	}
	else{
		startTask(stage1Control);
		startTask(stage2Control);
		//startTask(stallCheck);
		autonRan = false;
		//mobileGoal = false;
		//groundSetUpConeWait();
		getOutOfTheWayMid();
	}
	/**resetLiftEncoders();
	nMotorEncoder[intakeL]=0;
	startTask(dataLog);
	startTask(LCDTask);
	SensorValue(stage1Encoder)=0;
	startTask(coneControl);
	//startTask(driveControl);
	startTask(mobileGoalMotors);
  while(true){
  	if(vexRT[Btn8R]){
    	SensorValue(stage2Encoder)=0;
    	startTask(stage2Control);
    	startTask(stage1Control);
  	}
  	if(vexRT[Btn8U]){
    	desiredStage2=0;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn8R]){
    	desiredStage2=450;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn8D]){
    	desiredStage2=900;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn8L]){
    	desiredStage2=1350;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn7U]){
    	desiredStage1=0;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn7R]){
    	desiredStage1=200;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn7D]){
    	desiredStage1=400;
    	wait1Msec(200);
  	}
  	if(vexRT[Btn7L]){
    	desiredStage1=100;
    	wait1Msec(200);
  	}
	}*/
}
