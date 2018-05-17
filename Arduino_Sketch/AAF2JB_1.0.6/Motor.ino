//------------------------------------------------------------------
// Move the position by a single step if movement needed
//------------------------------------------------------------------
void doMovement() {
  if (targetPosition != currentPosition) {
    isMoving = true;

    // Adjust speed according to distance yet to travel
    int distance = currentPosition - targetPosition;
    if (abs(distance) > speedThreshold) {
      motorSpeed = highSpeedStepDelay;
    } else {
      motorSpeed = lowSpeedStepDelay;
    }

    // Going Anticlockwise to lower position
    if (targetPosition < currentPosition) {
      //for (int i = 0; i < motorStepsPerAscomStep; i++)
      anticlockwise();
    }

    // Going Clockwise to higher position

    if (targetPosition > currentPosition) {
      //for (int i = 0; i < motorStepsPerAscomStep; i++)
      clockwise();
    }

  } else {
    clearOutput();
    isMoving = false;

    // save new position in EEPROM if it has changed
    if (currentPosition != lastSavedPosition) {
      savePosition(currentPosition);
      lastSavedPosition = currentPosition;
    }

  }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Move stepper anticlockwise
//------------------------------------------------------------------
void anticlockwise()
{
  static unsigned long lastTime = micros();   // The last time the motor stepped.
  static int c = 0;                           // Counter to multiply ascom steps by steper steps.
  if (micros() - lastTime >= motorSpeed) {    // Check if it's time to step.
    lastTime = micros();                      // Reset step timer.
    step++;                                   // Go one step backwards
    if (step > 7) {                           // Check if the end of the stepping cycle was reached.
      step = 0;                               // Reset the stepping cycle.
    }
    SetOutput(step);                          // Send step signals to the ULN2003 driver.
    if (c < motorStepsPerAscomStep) {         // Multiply ascom steps by steper steps
      c++;
    } else {
      currentPosition--;                      // Increase the ASCOM position (0 - 10000)
      c = 0;                                  // Reset the counter.
    }
  }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Move stepper clockwise
//------------------------------------------------------------------
void clockwise()
{
  static unsigned long lastTime = micros();   // The last time the motor stepped.
  static int c = 0;                           // Counter to multiply ascom steps by steper steps.
  if (micros() - lastTime >= motorSpeed) {    // Check if it's time to step.
    lastTime = micros();                      // Reset step timer.
    step--;                                   // Go one step forwards
    if (step < 0) {                           // Check if the end of the stepping cycle was reached.
      step = 7;                               // Reset the stepping cycle.
    }
    SetOutput(step);                          // Send step signals to the ULN2003 driver.
    if (c < motorStepsPerAscomStep) {         // Multiply ascom steps by steper steps
      c++;
    } else {
      currentPosition++;                      // Increase the ASCOM position (0 - 10000)
      c = 0;                                  // Reset the counter.
    }
  }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Set output pins for stepper
//------------------------------------------------------------------
void SetOutput(int out)                   // Send step signals to the ULN2003 driver.
{
  for (int i = 0; i < 4; i++)
    digitalWrite(motorPins[i], bitRead(stepPattern[out], i));

  outputActive = true;
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Clear output pins for stepper
// To ensure they are not left in an on state after movement
//------------------------------------------------------------------
void clearOutput()
{
  if (outputActive) {               // only clear if already active
    for (int i = 0; i < 4; i++)
      digitalWrite(motorPins[i], 0);

    outputActive = false;
  }
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Save position in EEPROM - split into 2 byte values. Also sets position valid
//------------------------------------------------------------------
void savePosition(unsigned int p)
{
  byte lowByte = ((p >> 0) & 0xFF);
  byte highByte = ((p >> 8) & 0xFF);

  EEPROM.write(EE_LOC_POS, lowByte);
  EEPROM.write(EE_LOC_POS + 1, highByte);

  EEPROM.write(EE_LOC_PSTAT, POS_VALID);   // stored position is valid
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Restore position from EEPROM
//------------------------------------------------------------------
unsigned int restorePosition(void)
{
  byte lowByte = EEPROM.read(EE_LOC_POS);
  byte highByte = EEPROM.read(EE_LOC_POS + 1);

  return ((lowByte << 0) & 0xFF) + ((highByte << 8) & 0xFF00);
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Check if stored position in EEPROM is valid
//------------------------------------------------------------------
boolean storedPositionValid(void)
{
  byte status = EEPROM.read(EE_LOC_PSTAT);
  if (status == POS_VALID)
    return true;
  else
    return false;
}
//------------------------------------------------------------------
