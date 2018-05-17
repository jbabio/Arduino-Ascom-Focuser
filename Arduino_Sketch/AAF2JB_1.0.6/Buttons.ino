int CheckButton() {                 // Takes a pin number as parameter and returns a boolean. (HIGH: button pressed)

  static unsigned long lastDebounceTime = 0;      // *the last time the output pin was toggled
  static unsigned long debounceDelay = 50;        // *the debounce time; increase if the output flickers
  static int buttonState;                        // Reading from the button pin, static type to have global lifetime
  static int lastButtonState = LOW;              // *The previous reading from the button pin, static type to have global lifetime

  int reading = analogRead(buttonPin);            // *read the state of the switch into a local variable
  //Serial.println(reading);            // Uncomment this to show readed button values.
  int tmpButtonState = LOW;             // the current reading from the input pin

  for ( int i = 0 ; i < numButtons ; i++) {
    if (reading > buttonThresholdLow[i] && reading < buttonThresholdHigh[i]) {
      //Read switch 5
      tmpButtonState = button[i];
    }
  }

  if (tmpButtonState != lastButtonState) {  // If the switch changed, due to noise or pressing:
    lastDebounceTime = millis();            // reset the debouncing timer
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    // whatever the reading is at, it's been there for longer
    // than the debounce delay, so take it as the actual current state:
    buttonState = tmpButtonState;
    //Serial.println(buttonState);
  }

  // save the reading.  Next time through the loop,
  // it'll be the lastButtonState:
  lastButtonState = tmpButtonState;
  return tmpButtonState;
}

void ProcessButtons() {
  //static int focusStepSize = 1;               // Moved to Global Variables.
  switch (CheckButton()) {                      // Check if the button was pressed
    case 1:
        targetPosition = currentPosition + focusStepSize;
      break;
    case 2:
        static bool sleep = false;
        if (sleep == true){
          sleep = false;
          u8g2.sleepOff();
        }else{
          sleep = true;
          u8g2.sleepOn();
        }
      break;
    case 3:
      if (focusStepSize > 1) {
        focusStepSize = focusStepSize / 10;
      }
      break;
    case 4:
        targetPosition = currentPosition - focusStepSize;

      break;
    case 5:
      if (focusStepSize < 1000) {
        focusStepSize = focusStepSize * 10;
      }
      break;
  }
}

