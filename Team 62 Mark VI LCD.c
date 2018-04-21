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
  while(true){
    if(mode == 0){ //Main Mode
      clearLCDLine(0);
      clearLCDLine(1);
      displayLCDCenteredString(0, "62:Cameron/Miles");
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
      if(!autonRunning)
        displayLCDCenteredString(1, "<     Enter    >");
      else
        displayLCDCenteredString(1, "is currently running");

      if(choice == 0){ //Zero Auton
        displayLCDCenteredString(0, autons[0]);
      }
      if(choice == 1){ //First Auton
        displayLCDCenteredString(0, autons[1]);
      }
      if(choice == 2){ //Second Auton
        displayLCDCenteredString(0, autons[2]);
      }
      if(choice == 3){ //Third Auton
        displayLCDCenteredString(0, autons[3]);
      }
      if(choice == 4){ //Fourth Auton
        displayLCDCenteredString(0, autons[4]);
      }
      if(choice == 5){ //Fifth Auton
        displayLCDCenteredString(0, autons[5]);
      }
      if(choice == 6){ //Sixth Auton
        displayLCDCenteredString(0, autons[6]);
      }
      if(choice == 7){ //Seventh Auton
        displayLCDCenteredString(0, autons[7]);
      }
      if(choice == 8){ //Eighth Auton
        displayLCDCenteredString(0, autons[8]);
      }

      if(!autonRunning){ //Button Interface
        if(nLCDButtons == centerButton){
          autonNumber = choice;
          mode = 0;
          waitForReleaseLCD();
        }
        else if(nLCDButtons == leftButton){
          if(choice>0)
            choice--;
          else
            choice+=8;
          waitForReleaseLCD();
        }
        else if(nLCDButtons == rightButton){
          if(choice<8)
            choice++;
          else
            choice-=8;
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
