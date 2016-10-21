//------------------------------------------------------------------
// Get Temperature 
//------------------------------------------------------------------
double getTemperature()
{
  sensors.requestTemperatures();                // Get temperatures
  double tempC = sensors.getTempC(tempSensor);  // Get Temperature from our (single) Sensor
  
  if (tempC != -127.00)                         // if -127 then error so ignore
    currentTemperature = tempC;
  
  return currentTemperature;
}
//------------------------------------------------------------------

