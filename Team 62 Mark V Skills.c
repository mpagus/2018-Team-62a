#pragma systemFile

task drivebaseControlGyroSkills(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = -SensorValue(leftEncoder);
	float kP = 0.105;
	float kD = 0.081;
	float kPt = 0.33;
	float kDt = 0.0;
	float kPt2 = 0.13;
	float kDt2 = 0.58762;
	float error = 0;
	float errorT = 0;
	float derivative = 0;
	float derivativeT = 0;
	float driveSpeed = 0;
	float turnSpeed = 0;
	float old = 0;
	float oldT = 0;
	int slewVal = 15;
	while(true){
		errorT = desiredTurn - gyroLeftIsPositive()*SensorValue(Gyro);
		derivativeT = errorT - oldT;
		oldT = errorT;
		if(slewResetDrive){
			slewVal = 15;
			slewResetDrive = false;
		}
		if(!straight){
			drive(slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 6), -slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT), 6));
		}
		else{
			//avgEncoder = (SensorValue(rightEncoder) + SensorValue(leftEncoder)) / 2.0;
			//error = desiredDrive - avgEncoder;
			error = desiredDrive + SensorValue(leftEncoder);
			derivative = error - old;
			old = error;
			driveSpeed = slew(&slewVal, limit(kP*(error) + kD*derivative), 10);
			turnSpeed = limit(kPt*errorT + kDt*derivativeT, -1.75*abs(driveSpeed), 1.75*abs(driveSpeed));
			drive(driveSpeed + turnSpeed, driveSpeed - turnSpeed);
		}
		delay(5);
	}
}

int stepWait = 35;

void resetGyro10PtBar(){
	float oldDesiredDrive = desiredDrive;
	stopTask(drivebaseControlGyroSkills);
	drive(35, 35);
	float oldVal = SensorValue(leftEncoder);
	while(!deadband2(oldVal, SensorValue(leftEncoder), 15)){
		oldVal = SensorValue(leftEncoder);
		wait1Msec(45);
	}
	wait1Msec(300);
	SensorValue(Gyro) = 0;
	desiredTurn = 0;
	drive(0, 0);
	startTask(drivebaseControlGyroSkills);
	desiredDrive = -SensorValue(leftEncoder);
}

void goal1(){
	mobileGoal = false;
	wait1Msec(stepWait);
	driveBothWait(1500);
	mobileGoalIn();
	moveStage2WaitUntil(90, 60);
	wait1Msec(300);
	getOutOfTheWayLow();
	driveBothWait(-1300);
	turn45DegreesR();
	wait1Msec(stepWait);
	driveBothWait(-640);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(850, 250);
	mobileGoal = false;
	driveBothWait(0);
	wait1Msec(stepWait+300);
}

void goal2(){
	mobileGoal = true;
	driveBothWaitUntil(-1000, -500);
	driveBothWait(0);
	mobileGoal = false;
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(330);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(900);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turnWait(1820);
	wait1Msec(stepWait);
	driveBothWaitUntil(1130, 350);
	mobileGoal = false;
	driveBothWait(0);
	//resetGyro10PtBar();
}

void goal3(){
	mobileGoal = true;
	driveBothWait(-330);
	wait1Msec(stepWait);
	//mobileGoalIn();
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(420);
	wait1Msec(stepWait);
	turnWait(-510);
	mobileGoal = false;
	wait1Msec(stepWait+150);
	//mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(1270);
	wait1Msec(stepWait);
	mobileGoal = true;
	wait1Msec(stepWait+400);
	driveBothWait(-1270);
	wait1Msec(stepWait);
	turn45DegreesL();
	wait1Msec(stepWait);
	driveBothWait(-665);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	mobileGoal = false;
	driveBothWait(300);
	wait1Msec(stepWait);
	resetGyro10PtBar();
	wait1Msec(stepWait);
}

void goal4(){
	driveBothWaitUntil(-380, -170);
	mobileGoal = true;
	driveBothWait(0);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	mobileGoal = false;
	driveBothWait(335);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(855);
	wait1Msec(stepWait);
	mobileGoalIn();
	turn180DegreesL();
	driveBothWaitUntil(960, 420);
	mobileGoal = false;
	driveBothWait(0);
}

void goal5(){
	driveBothWait(-550);
	turn180DegreesL();
	turnWait(15);
	driveBothWaitUntil(2480, 1810);
	mobileGoal = true;
	driveBothWait(0);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(465);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(920, 360);
	mobileGoal = false;
	driveBothWait(0);
}

void goal6(){
	driveBothWaitUntil(-920, -300);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(-500);
	wait1Msec(stepWait);
	turn90DegreesR();
	driveBothWait(665);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn180DegreesL();
	wait1Msec(stepWait);
	driveBothWait(840);
	wait1Msec(stepWait);
	resetGyro10PtBar();
	mobileGoal = false;
	wait1Msec(1000);
}

void goal7(){
	driveBothWaitUntil(-285, -170);
	mobileGoal = true;
	driveBothWait(0);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(490);
	wait1Msec(stepWait);
	turn45DegreesL();
	mobileGoal = false;
	driveBothWait(1200);
	wait1Msec(stepWait);
	mobileGoal = true;
	wait1Msec(stepWait+400);
	driveBothWait(-1150);
	wait1Msec(stepWait);
	turn45DegreesR();
	wait1Msec(stepWait);
	driveBothWait(-700);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(350,0);
	mobileGoal = false;
	driveBothWait(0);
}

void endGoal7(){
	driveBothWait(-285);
	wait1Msec(stepWait);
	//mobileGoalIn();
	wait1Msec(stepWait);
}

void goal8(){

}

void auton(){
	resetEncoders();
	resetLiftEncoders();
	datalogClear();
	datalogStart();
	startTask(dataLog);
	startTask(drivebaseControlGyroSkills);
	startTask(mobileGoalAuton);
	autonRan = true;
	startTask(stage1Control);
	startTask(stage2Control);
	unfoldRobotAuton();

	goal1();
	goal2();
	goal3();
	goal4();
	goal5();
	goal6();
	goal7();
	endGoal7();
	//goal8();

	datalogStop();
}

void testPID(){
	resetEncoders();
	startTask(drivebaseControlGyroSkills);
	driveBothWait(1200);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1200);
	wait1Msec(1100);
	turnWait(1800);
	wait1Msec(1100);
	driveBothWait(1200);
	//driveBothWait(100000000);
}

void testTurn(){
	startTask(drivebaseControlGyroSkills);
	resetEncoders();
	turn90DegreesR();
	wait1Msec(1000);
	turn90DegreesL();
	wait1Msec(1000);
	turn45DegreesR();
	wait1Msec(1000);
	turn45DegreesL();
	wait1Msec(1000);
	turn180DegreesL();
}
