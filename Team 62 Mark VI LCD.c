const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;

int num = 0;

void waitForPressLCD(){
	while(nLCDButtons == 0){wait1Msec(25);}
}

void waitForReleaseLCD(){
	while(nLCDButtons != 0){wait1Msec(25);}
	num++;
}

task LCDTask(){
  bLCDBacklight = true;
  string mainBattery;
  string backupBattery;
  string output;
  int mode = 0;
  int choice = 0;
  int num = 0;
  int showAuton = false;
  int additionalNum = 0;
  while(true){
    if(mode == 0){ //Main Mode
      clearLCDLine(0);
      clearLCDLine(1);
      if(num<25){
        num++;
      }
      else{
        num=0;
        showAuton=!showAuton;
      }
      if(!showAuton){
        displayLCDCenteredString(0, "62:Cameron/Miles");
      }
      else{
        displayLCDString(0, 0, "62:");
        if(choice==0){
        	displayNextLCDString("None");
      	}
      	else{
					displayNextLCDString(autons[autonCatagory][0]);
	        displayNextLCDString(autons[autonCatagory][choice]);
	        if(autonRight)
	        	displayNextLCDString("R");
	        else
	        	displayNextLCDString("L");
	      }
      }
      displayLCDString(1, 0, "Auton Batts Vars");

      if(nLCDButtons == leftButton){ //Auton Mode
        mode = 2;
        choice = -1;
        waitForReleaseLCD();
      }
      if(nLCDButtons == centerButton){ //Battery Mode
        mode = 1;
        waitForReleaseLCD();
      }
      if(nLCDButtons == rightButton){ //Visualizer Mode
        mode = 3;
        waitForReleaseLCD();
      }
    }
    if(mode == 1){ //Batery Mode
      clearLCDLine(0);
      clearLCDLine(1);

      //Display the Primary Robot battery voltage
      displayLCDString(0, 0, "Cortex: ");
      sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V');
      displayNextLCDString(mainBattery);

      //Display the Secondary Robot battery voltage
      displayLCDString(1, 0, "Power X: ");
      sprintf(backupBattery, "%1.2f%c", SensorValue(PowerExpander)/70, 'V'); //or 280
      displayNextLCDString(backupBattery);

      //Exit from mode
      if(nLCDButtons != 0){
        mode = 0;
      }
    }
    if(mode == 2){ //Autonomous Mode
      clearLCDLine(0);
      clearLCDLine(1);
      if(autonRan){
        displayLCDString(0,0,autons[autonCatagory][0]);
        displayNextLCDString(autons[autonCatagory][choice]);
        displayLCDCenteredString(1, "is currently running");
      }

      if(!autonRan){ //Button Interface
        if(choice==-1){
          displayLCDCenteredString(0, "Choose A Side");
          displayLCDCenteredString(1, "L    None      R");
          if(nLCDButtons == leftButton){
            choice = 0;
            autonRight = false;
            waitForReleaseLCD();
          }
          else if(nLCDButtons == centerButton){
            choice = 0;
            mode = 0;
            autonNumber = 0;
            waitForReleaseLCD();
          }
          else if(nLCDButtons == rightButton){
            choice = 0;
            autonRight = true;
            waitForReleaseLCD();
          }
        }
        else if(choice==0){
          displayLCDCenteredString(0, autons[additionalNum][0]);
          displayLCDCenteredString(1, "<    Enter     >");
          if(nLCDButtons == leftButton){
            if(additionalNum>0)
              additionalNum--;
            else
              additionalNum=3;
            waitForReleaseLCD();
          }
          else if(nLCDButtons == centerButton){
            autonCatagory = additionalNum;
            choice = 1;
            waitForReleaseLCD();
          }
          else if(nLCDButtons == rightButton){
            if(additionalNum<3)
              additionalNum++;
            else
              additionalNum=0;
            waitForReleaseLCD();
          }
        }
        else{
          displayLCDCenteredString(0, autons[additionalNum][choice]);
          displayLCDCenteredString(1, "<    Enter     >");
          if(nLCDButtons == leftButton){
            if(choice>1)
              choice--;
            else{
              while(autons[additionalNum][choice+1]!=""){
                choice++;
              }
            }
            waitForReleaseLCD();
          }
          else if(nLCDButtons == centerButton){
            autonNumber = choice;
            mode=0;
            waitForReleaseLCD();
            bLCDBacklight = false;
          }
          else if(nLCDButtons == rightButton){
            if(autons[additionalNum][choice+1]!="")
              choice++;
            else
              choice=1;
            waitForReleaseLCD();
          }
        }
      }
    }
    if(mode == 3){ //Variable Visualizer Mode
      clearLCDLine(0);
      clearLCDLine(1);

      displayLCDCenteredString(0, "LCDVar1: ");
      displayNextLCDNumber(LCDVar1);
      displayLCDCenteredString(1, "LCDVar2: ");
      displayNextLCDNumber(LCDVar2);

      if(nLCDButtons != 0){
        mode = 0;
        waitForReleaseLCD();
      }
    }
    wait1Msec(100);
  }
}
