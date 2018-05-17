//-------------------------------------------------------------------------------
// Built-in serialEvent occurs whenever new data comes in the Hardware Serial
// This is called automatically between each loop() run
//-------------------------------------------------------------------------------
void serialEvent() {
  while (Serial.available()) {
    char inChar = (char)Serial.read();        // get the new byte:
    inputString += inChar;                    // add it to the inputString:

    if (inChar == '#') {                      // if incoming character is hash, set flag so main loop can action it
      stringComplete = true;
    }
  }
}
//-------------------------------------------------------------------------------


//------------------------------------------------------------------
// Do commands incoming from USB serial port
//------------------------------------------------------------------
void doCommand(String command) {

  switch (command.charAt(0)) {
    case '#':  // Confirm Connection
      Serial.print("OK!#");
      break;
    case 'T': // Set Target Position
      {
        int hashpos = command.indexOf('#');    // position of hash in string
        String targetPosS = command.substring(1, hashpos);
        unsigned int targetPosI = targetPosS.toInt();
        targetPosition = targetPosI;
        Serial.print("T" + targetPosS + ":OK#");
        break;
      }
    case 'C': // Get Temperature
      {

        double t;

        // if moving block temperature requests as they interfere with movement. Just return last reading.
        if (isMoving)
          t = currentTemperature;
        else
          t = getTemperature();

        Serial.print("C");
        Serial.print(t * 100, 0);
        Serial.print(":OK#");
        break;
      }
    case 'I': // Set Initial Position. Sets Position without any movement
      {
        int hashpos = command.indexOf('#');    // position of hash in string
        String initPosS = command.substring(1, hashpos);
        unsigned int initPosI = initPosS.toInt();
        currentPosition = initPosI;
        targetPosition = initPosI;
        Serial.print("I" + initPosS + ":OK#");
        break;
      }
    case 'P': // Get Current Position
      {
        String currentPositionS = String(currentPosition);
        Serial.print("P" + currentPositionS + ":OK#");
        break;
      }
    case 'H': // Halt
      {
        targetPosition = currentPosition;
        String currentPositionS = String(currentPosition);
        Serial.print("H" + currentPositionS + ":OK#");
        break;
      }
    case 'M': // Is motor moving
      {
        if (isMoving) {
          Serial.print("M1:OK#");
        } else {
          Serial.print("M0:OK#");
        }
        break;
      }
    case 'V': // Get Version and abilities
      {
        String tempInstalled = (tempSensorPresent ? " | Temp. Sensor |" : "");
        String posValid = (storedPositionValid() ? " | Stored Position |" : "");

        Serial.print( programName + " V" + programVersion + tempInstalled + posValid + "#");
        break;
      }
    default:
      {
        motorSpeed = defaultStepDelay;
        Serial.print("ERR#");
        break;
      }
  }

  // clear the command string and flag
  inputString = "";
  stringComplete = false;
}
//------------------------------------------------------------------
