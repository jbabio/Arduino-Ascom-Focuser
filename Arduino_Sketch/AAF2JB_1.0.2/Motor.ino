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
      for (int i=0; i < motorStepsPerAscomStep; i++)
        anticlockwise();
      
      currentPosition--;
    }
    
    // Going Clockwise to higher position
    if (targetPosition > currentPosition) {
      for (int i=0; i < motorStepsPerAscomStep; i++)
        clockwise();
      
      currentPosition++;
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
  step++;
  if (step > 7)
    step = 0;

  setOutput(step);
  delayMicroseconds(motorSpeed);
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Move stepper clockwise 
//------------------------------------------------------------------
void clockwise()
{
  step--;
  if (step < 0)
    step = 7;
    
  setOutput(step);
  delayMicroseconds(motorSpeed);
}
//------------------------------------------------------------------

//------------------------------------------------------------------
// Set output pins for stepper
//------------------------------------------------------------------
void setOutput(int out)
{
  for (int i=0; i<4; i++)
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
    for (int i=0; i<4; i++) 
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
