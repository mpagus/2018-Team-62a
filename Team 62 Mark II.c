#pragma config(Sensor, in6,    sivePot,        sensorPotentiometer)
#pragma config(Sensor, in7,    barEncoder,     sensorPotentiometer)
#pragma config(Sensor, in8,    intakeEncoder,  sensorPotentiometer)
#pragma config(Sensor, dgtl1,  ripperEncoder,  sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  highGoalLight,  sensorDigitalOut)
#pragma config(Sensor, dgtl4,  ,               sensorDigitalOut)
#pragma config(Motor,  port1,           ripperBarL,    tmotorVex393HighSpeed_HBridge, openLoop, reversed)
#pragma config(Motor,  port2,           ripperR,       tmotorVex393TurboSpeed_MC29, openLoop)
#pragma config(Motor,  port3,           ripperL,       tmotorVex393TurboSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port4,           intakeR,       tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           ripperBarR,    tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port6,           siveM,         tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           leftDrive,     tmotorVex393HighSpeed_MC29, openLoop)
#pragma config(Motor,  port8,           topRollerMotor, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightDrive,    tmotorVex393HighSpeed_MC29, openLoop, reversed)
#pragma config(Motor,  port10,          intakeL,       tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

// This code is for the VEX cortex platform
#pragma platform(VEX2)

// Select Download method as "competition"
#pragma competitionControl(Competition)

//Main competition background code...do not modify!
#include "Vex_Competition_Includes.c"

float desiredRipper = 0;
float desiredBar = 0;
bool mobileGoal = true;
bool highGoal = false;
int currentConeStack = 0;
int currentHighConeStack = 0;
bool sive = false;

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

void ripperBar(int val){
	motor[ripperBarR] = val;
	motor[ripperBarL] = val;
}

void ripper(int val){
	motor[ripperR] = motor[ripperL] = val;
}

void topRoller(int val){
	motor[topRollerMotor] = val;
}

void intake(int val){
	motor[intakeR] = motor[intakeL] = val;
}

void siveMotor(int val){
	motor[siveM] = val;
}

void drive(int rVal, int lVal){
	motor[rightDrive] = rVal;
	motor[leftDrive] = lVal;
}

void driveDeadband(int rVal, int lVal, int db = 10){
	if(deadband(rVal, 0, db))
		motor[rightDrive] = rVal;
	else
		motor[rightDrive] = 0;
	if(deadband(lVal, 0, db))
		motor[leftDrive] = lVal;
	else
		motor[leftDrive] = 0;
}

task driveControl(){
	while(true){
		driveDeadband(vexRT[Ch2], vexRT[Ch3], 10);
		delay(5);
	}
}

task mobileGoalMotors(){
	bool accurate = false;
	while(true){
		if(mobileGoal){
			if(SensorValue(intakeEncoder) < 2200)
	    	intake(127);
	    else if(SensorValue(intakeEncoder) < 2600)
	      intake(20);
	  	else
	  		intake(0);
		}
		else{
			if(SensorValue(intakeEncoder) > 700)
	     	intake(-127);
	    //else if(SensorValue(intakeEncoder) > 850)
	      //intake(-30);
	  	else
	  		intake(-30);
	  }
		if(vexRT[Btn8D] != accurate){
			accurate = !accurate;
			if(accurate)
				mobileGoal = !mobileGoal;
		}
		delay(5);
	}
}

task ripperControl(){
    desiredRipper = SensorValue(ripperEncoder);
    float kP = 0.25;
    float error = 0;
    while(true){
        error = desiredRipper - SensorValue(ripperEncoder);
        ripper(limit(error * kP));
        delay(5);
    }
}

task barControl(){
    desiredBar = SensorValue(barEncoder);
    float kP = 0.09;
    float error = 0;
    while(true){
        error = desiredBar - SensorValue(barEncoder);
        ripperBar(limit(error * kP));
        delay(5);
    }
}
//siv3
task siveMotors(){
	bool accurate = false;
	while(true){
		if(vexRT[Btn6U] || vexRT[Btn6D]){
			siveMotor(120);
			wait1Msec(80);
		}
		else if(sive && SensorValue(sivePot) > 950)
			siveMotor(127);
		else if(sive)
			siveMotor(0);
		else if(!sive && SensorValue(sivePot) < 3900)
			siveMotor(-127);
		else
			siveMotor(-7);
		if(vexRT[Btn5U] != accurate && vexRT[Btn5D] != accurate){
			accurate = !accurate;
			if(accurate){
				sive = !sive;
			}
			delay(5);
		}
	}
}

int normalStackVal(int cone){
		return 85*cone + 400;
}
//rip3
void normalStackCone(int cone){
		wait1Msec(20);
		desiredRipper = normalStackVal(cone);
    wait1Msec(260+33*cone);
		desiredBar = 3380; //four bar outtake
    wait1Msec(275+28*cone);//<this is the fucked one
    stopTask(ripperControl);
    ripper(-50);
    wait1Msec(150);
    desiredBar = 2640; //intakeUp
    wait1Msec(200);
    ripper(0);
    //wait1Msec(200);
    startTask(ripperControl);
    desiredRipper = 300;
}

void highStackCone(int cone){
    //IDK IF WE CAN DO THIS
}

void intakeConeRipper(){
    desiredRipper = 300;
    desiredBar = 1850;//intakeDown
    wait1Msec(300);
    desiredRipper = 0;
}

void topRollerRipper(){
    desiredRipper = 300;
 		wait1Msec(200);
    desiredBar = 2640;//intakeUp
}

void preloadPickUpCone(){
    //IDK IF WE CAN DO THIS
}

void setupLift(){
    //drive(50,50);
    //wait1Msec(200);
    //drive(0,0);
    ripper(-35);
    float old = SensorValue(ripperEncoder);
    wait1Msec(200);
    while(SensorValue(ripperEncoder)<old){
    		old = SensorValue(ripperEncoder);
    		wait1Msec(150);
  	}
  	wait1Msec(400);
    ripper(0);
    SensorValue[ripperEncoder]=0;
}

task coneControl(){
	currentConeStack = 0;
	currentHighConeStack = 0;
	bool rollerCone = false;
	bool intakeLowered = true;
	highGoal = false;
	bool highGoalAccurate = false;
	bool messUpButton = false;
	bool incButton = false;
	bool ripButton = false;
	while(true){
		if(!highGoal){
			if(vexRT[Btn8L]){
				currentConeStack = 0;
			}
			if(vexRT[Btn8R] != messUpButton){
				messUpButton = !messUpButton;
				if(messUpButton && currentConeStack > 0){
					currentConeStack--;
				}
			}
			if(vexRT[Btn8U] != incButton){
				incButton = !incButton;
				if(incButton && currentConeStack < 9){
					currentConeStack++;
				}
			}
			if(vexRT[Btn6U] != ripButton){
				ripButton = !ripButton;
				if(currentConeStack < 10 && ripButton){
					currentConeStack++;
					normalStackCone(currentConeStack);
					intakeLowered = false;
				}
			}
			if(vexRT[Btn6D] != rollerCone){
				rollerCone = !rollerCone;
				if(rollerCone){
					if(intakeLowered){
						topRollerRipper();
						intakeLowered = false;
					}
					else{
						intakeConeRipper();
						intakeLowered = true;
					}
				}
			}
		}
		if(highGoal){
			if(vexRT[Btn8D]){
				currentHighConeStack = 0;
			}
			if(vexRT[Btn8R] != messUpButton){
				messUpButton = !messUpButton;
				if(messUpButton && currentHighConeStack > 0){
					currentHighConeStack--;
				}
			}
			if(vexRT[Btn8U] != incButton){
				incButton = !incButton;
				if(incButton && currentHighConeStack < 7){
					currentHighConeStack++;
				}
			}
			if(vexRT[Btn6U] != ripButton){
				ripButton = !ripButton;
				if(currentHighConeStack < 7 && ripButton){
					currentHighConeStack++;
					highStackCone(currentHighConeStack);
					intakeLowered = false;
				}
			}
			if(vexRT[Btn6D] != rollerCone){
				rollerCone = !rollerCone;
				if(rollerCone){
					if(intakeLowered){
						topRollerRipper();
						intakeLowered = false;
					}
					else{
						intakeConeRipper();
						intakeLowered = true;
					}
				}
			}
		}
		//if(vexRT[Btn8D] != highGoalAccurate){
			//highGoalAccurate = !highGoalAccurate;
			//if(highGoalAccurate){
				//highGoal = !highGoal;
				//SensorValue(highGoalLight) = highGoal;
			//}
		//}
		if(vexRT[Btn5U] && !vexRT[Btn5D]){
			topRoller(127);
		}
		else if(vexRT[Btn5D] && !vexRT[Btn5U]){
			topRoller(-127);
		}
		else{
			topRoller(0);
		}
	}
}

void pre_auton() {
	wait1Msec(100);
}

task autonomous(){
	pre_auton();
}

task usercontrol(){
	setupLift();
	//SensorValue[ripperEncoder]=0;
	startTask(coneControl);
	startTask(driveControl);
	startTask(ripperControl);
	startTask(barControl);
	startTask(mobileGoalMotors);
	startTask(siveMotors);
}
