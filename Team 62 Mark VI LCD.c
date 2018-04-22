const short leftButton = 1;
const short centerButton = 2;
const short rightButton = 4;

void waitForPressLCD(){
	while(nLCDButtons == 0){wait1Msec(5);}
}

void waitForReleaseLCD(){
	while(nLCDButtons != 0){wait1Msec(5);}
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
				displayNextLCDString(autons[autonNumber])
      }
      //sprintf(output, "Auton", autonNumber, " Batt Vars");
      //displayLCDString(1, 0, output);
      displayLCDString(1, 0, "Auton Batts Vars");

      if(nLCDButtons == leftButton){ //Auton Mode
        mode = 2;
        choice = autonNumber;
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
      displayLCDString(0, 0, "Batt 1: ");
      sprintf(mainBattery, "%1.2f%c", nImmediateBatteryLevel/1000.0,'V');
      displayNextLCDString(mainBattery);

      //Display the Secondary Robot battery voltage
      displayLCDString(1, 0, "Batt 2: ");
      sprintf(backupBattery, "%1.2f%c", SensorValue(PowerExpander)/1000.0, 'V');
      displayNextLCDString(backupBattery);

      //Exit from mode
      if(nLCDButtons != 0){
        mode = 0;
      }
    }
    if(mode == 2){ //Autonomous Mode
      clearLCDLine(0);
      clearLCDLine(1);
      if(!autonRan)
        displayLCDCenteredString(1, "<     Enter    >");
      else
        displayLCDCenteredString(1, "is currently running");

      displayLCDCenteredString(0, autons[choice]);

      if(!autonRunning){ //Button Interface
        if(nLCDButtons == centerButton){
          autonNumber = choice;
          mode = 0;
          bLCDBacklight = false;
          waitForReleaseLCD();
        }
        else if(nLCDButtons == leftButton){
          if(choice > 0)
            choice--;
          else
            choice = choice + (sizeOf(autons) - 1);
          waitForReleaseLCD();
        }
        else if(nLCDButtons == rightButton){
          if(choice < sizeOf(autons) - 1)
            choice++;
          else
            choice = choice - (sizeOf(autons) - 1);
          waitForReleaseLCD();
        }
      }
    }
    if(mode == 3){ //Variable Visualizer Mode
      clearLCDLine(0);
      clearLCDLine(1);

      sprintf(output, "LCDVar1: ", LCDVar1);
      displayLCDCenteredString(0, output);
      sprintf(output, "LCDVar2: ", LCDVar2);
      displayLCDCenteredString(1, output);

      if(nLCDButtons != 0){
        mode = 0;
        waitForReleaseLCD();
      }
    }
    wait1Msec(100);
  }
}
