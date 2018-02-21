#pragma systemFile

task drivebaseControlGyroSkills(){
	//desiredDrive = (SensorValue(leftEncoder) + SensorValue(rightEncoder)) / 2;
	desiredDrive = SensorValue(leftEncoder);
	float kP = 0.115;
	float kD = 0.081;
	float kPt = 0.8201;
	float kDt = 0.0;
	float kPt2 = 0.1114;
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

int stepWait = 350;

void goal1(){
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(1500);
	mobileGoalIn();
	wait1Msec(stepWait);
	driveBothWait(-1050);
	wait1Msec(550);
	turn45DegreesR();
	wait1Msec(stepWait);
	driveBothWait(-840);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(1100, 800);
	mobileGoalOut();
	wait1Msec(stepWait);
}

void goal2(){
	driveBothWaitUntil(-750, -675);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWaitUntil(400, 350);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(675);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn180DegreesL();
	wait1Msec(stepWait);
	driveBothWait(1000);
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
}

void goal3(){
	driveBothWait(-300);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(425);
	wait1Msec(stepWait);
	turn45DegreesR();
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(1175);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	driveBothWait(-1150);
	wait1Msec(stepWait);
	turn45DegreesL();
	wait1Msec(stepWait);
	driveBothWait(-950);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(300);
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
}

void goal4(){
	driveBothWait(-300);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(575);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(650);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn180DegreesL();
	wait1Msec(stepWait);
	driveBothWait(1075);
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
}

void goal5(){
	driveBothWait(-350);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn180DegreesL();
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(1775);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	driveBothWait(850);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(375);
	wait1Msec(stepWait);
	turn90DegreesR();
	wait1Msec(stepWait);
	driveBothWait(675);
	wait1Msec(stepWait);
	mobileGoalOut();
}

void goal6(){
	driveBothWait(-675);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	driveBothWait(375);
	wait1Msec(stepWait);
	turn90DegreesL();
	wait1Msec(stepWait);
	mobileGoalOut();
	wait1Msec(stepWait);
	driveBothWait(675);
	wait1Msec(stepWait);
	mobileGoalIn();
	wait1Msec(stepWait);
	turn180DegreesL();
	wait1Msec(stepWait);
	driveBothWait(975);
	wait1Msec(stepWait);
	mobileGoalOut();
}

void goal7(){

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
	//goal7();
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
