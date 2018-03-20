#pragma systemFile

task drivebaseControlGyroSkills(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = -SensorValue(leftEncoder);
	float kP = 0.19;
	float kD = 1.48;
	float kPt = 0.25;
	float kDt = 0.0;
	float kPt2 = 0.30;
	float kDt2 = 1.47;
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
			drive(slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT, -100, 100), 3, 15), -slew(&slewVal, limit(kPt2*errorT + kDt2*derivativeT, -100, 100), 3, 15));
		}
		else{
			//avgEncoder = (SensorValue(rightEncoder) + SensorValue(leftEncoder)) / 2.0;
			//error = desiredDrive - avgEncoder;
			error = desiredDrive + SensorValue(leftEncoder);
			derivative = error - old;
			old = error;
			driveSpeed = limit(kP*(error) + kD*derivative, -100, 100);
			turnSpeed = limit(kPt*errorT + kDt*derivativeT, -1.75*abs(driveSpeed), 1.75*abs(driveSpeed));
			drive(driveSpeed + turnSpeed, driveSpeed - turnSpeed);
		}
		batteryPower = nImmediateBatteryLevel;
		delay(15);
	}
}

int stepWait = 0;

void resetGyro10PtBar(){
	float oldDesiredDrive = desiredDrive;
	stopTask(drivebaseControlGyroSkills);
	drive(30, 30);
	float oldVal = SensorValue(leftEncoder);
	while(!deadband2(oldVal, SensorValue(leftEncoder), 2)){
		oldVal = SensorValue(leftEncoder);
		wait1Msec(45);
	}
	wait1Msec(200);
	SensorValue(Gyro) = 0;
	desiredTurn = 0;
	drive(0, 0);
	startTask(drivebaseControlGyroSkills);
	desiredDrive = -SensorValue(leftEncoder);
}

void goal1(){
	mobileGoal = false;
	wait1Msec(450);
	driveBothWait(1230);
	mobileGoalIn();
	desiredDrive -= 1260;
	wait1Msec(400);
	desiredStage2 = 850;
	wait1Msec(400);
	desiredStage1 = 220;
	desiredStage2 = 1080-370;
	driveBothWait(0);
	turn45DegreesR();
	driveBothWait(-610);
	turn90DegreesR();
	driveBothWaitUntil(810, 570);
	mobileGoalOut();
	wait1Msec(300);
	//driveBothWait(0);
}

void goal2(){
	mobileGoal = true;
	driveBothWait(-720);
	mobileGoal = false;
	driveBothWait(0);
	turn90DegreesR();
	driveBothWait(425);
	turnWait(-870);
	driveBothWait(590);
	mobileGoalIn();
	turnWait(1820);
	driveBothWaitUntil(655, 440);
	mobileGoal = false;
	driveBothWait(0);
}

void goal3(){
	driveBothWait(-260);
	mobileGoal = true;
	turn90DegreesR();
	driveBothWait(360);
	mobileGoal = false;
	turnWait(-435);
	wait1Msec(100);
	driveBothWait(850);
	mobileGoalIn();
	wait1Msec(400);
	driveBothWait(-890);
	turn45DegreesL();
	driveBothWait(-610);
	turn90DegreesL();
	mobileGoal = false;
	driveBothWait(180);
	resetGyro10PtBar();
}

void goal4(){
	driveBothWaitUntil(-220, -140);
	mobileGoal = true;
	driveBothWait(0);
	turn90DegreesL();
	mobileGoal = false;
	driveBothWait(350);
	turn90DegreesL();
	driveBothWait(615);
	mobileGoalIn();
	turn180DegreesL();
	driveBothWaitUntil(750, 460);
	mobileGoal = false;
	driveBothWait(0);
}

void goal5(){
	mobileGoal = false;
	driveBothWait(-550);
	turn180DegreesL();
	turnWait(-15);
	driveBothWait(700);
	mobileGoal = true;
	wait1Msec(400);
	driveBothWait(650);
	turn90DegreesL();
	driveBothWait(220);
	turn90DegreesR();
	driveBothWaitUntil(700, 480);
	mobileGoal = false;
	driveBothWait(0);
}

void goal6(){
	driveBothWaitUntil(-750, -300);
	mobileGoal = true;
	driveBothWait(0);
	mobileGoal = false;
	turn90DegreesR();
	driveBothWait(-320);
	turn90DegreesR();
	driveBothWait(500);
	mobileGoalIn();
	turn180DegreesL();
	driveBothWaitUntil(600, 310);
	mobileGoal = false;
	driveBothWait(0);
	resetGyro10PtBar();
	mobileGoalOut();
}

void goal7(){
	driveBothWaitUntil(-285, -170);
	mobileGoal = true;
	driveBothWait(0);
	turn90DegreesL();
	driveBothWait(380);
	turn45DegreesL();
	mobileGoal = false;
	driveBothWait(750);
	mobileGoal = true;
	wait1Msec(500);
	driveBothWait(-750);
	turn45DegreesR();
	driveBothWait(-720);
	turn90DegreesR();
	driveBothWaitUntil(350,0);
	mobileGoal = false;
	driveBothWait(0);
}

void goal8(){

}

void endGoal6(){
	driveBothWaitUntil(-285, -170);
	mobileGoal = true;
	driveBothWait(0);
	turn90DegreesL();
	driveBothWait(380);
	turn45DegreesL();
	driveBothWait(600);
}

void endGoal7(){
	driveBothWait(-285);
}

void auton(){
	resetEncoders();
	resetLiftEncoders();
	datalogClear();
	datalogStart();
	startTask(stage1Control);
	startTask(stage2Control);
	startTask(dataLog);
	startTask(drivebaseControlGyroSkills);
	startTask(mobileGoalAuton);
	autonRan = true;
	unfoldRobotAuton();
	goal1();
	goal2();
	goal3();
	goal4();
	goal5();
	goal6();
	endGoal6();
	//goal7();
	//endGoal7();
	//goal8();

	datalogStop();
}

void testPID(){
	resetEncoders();
	startTask(drivebaseControlGyroSkills);
	driveBothWait(1000);
	//wait1Msec(1100);
	turnWait(1800);
	//wait1Msec(1100);
	driveBothWait(1000);
	//wait1Msec(1100);
	turnWait(1800);
	//wait1Msec(1100);
	driveBothWait(1000);
	turnWait(1800);
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
