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
void getAmbientData() {
  BME280::TempUnit tempUnit(BME280::TempUnit_Celsius);
  BME280::PresUnit presUnit(BME280::PresUnit_Pa);
  EnvironmentCalculations::TempUnit     envTempUnit =  EnvironmentCalculations::TempUnit_Celsius;
  bme.read(pres, temp, hum, tempUnit, presUnit);
  dewPoint = EnvironmentCalculations::DewPoint(temp, hum, envTempUnit);
}

