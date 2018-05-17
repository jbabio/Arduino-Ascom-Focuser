  void draw() {
  u8g.setRot180();
  u8g.setFont(u8g_font_helvB08);
  // Telescope tube temperature
  u8g.drawStr( 0, 10, "Tube:");
  u8g.drawStr( 30, 10, dtostrf(currentTemperature, 5, 2, str));
  u8g.drawStr( 58, 10, "\260C");
  // Air temperature
  u8g.drawStr( 0, 21, "Air:");
  u8g.drawStr( 20, 21, "na.na");//dtostrf(temperature, 5, 2, str));
  u8g.drawStr( 48, 21, "\260C");
  // Humidity
  u8g.drawStr( 64, 21, "Hum:");
  u8g.drawStr( 90, 21, "na.na");//dtostrf(humidity, 5, 2, str));
  u8g.drawStr( 120, 21, "%");
  // Focuser position
  u8g.drawStr( 0, 32, "Pos:");
  u8g.drawStr( 25, 32, dtostrf(currentPosition, 4, 0, str));
  // Target position
  u8g.drawStr( 64, 32, "Targ:");
  u8g.drawStr( 91, 32, dtostrf(targetPosition, 4, 0, str));
  // Buttons read
  u8g.drawStr( 0, 43, "Focus step:");
  u8g.drawStr( 60, 43, dtostrf(focusStepSize, 4, 0, str));
  // Motor Status
  u8g.setFont(u8g_font_courB18);
  if (isMoving) {
    u8g.drawStr( 0, 64, "MOVING");
  } else {
    u8g.drawStr( 0, 64, "IDLE");
  }
}
