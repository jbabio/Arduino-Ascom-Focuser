  void draw() {
  u8g.setRot180();
  u8g.setFont(u8g_font_helvB08);
  // Telescope tube temperature
  u8g.drawStr( 0, 10, "Tube:");
  u8g.drawStr( 30, 10, dtostrf(currentTemperature, 5, 2, str));
  u8g.drawStr( 58, 10, "\260C");
  // Air temperature
  u8g.drawStr( 0, 20, "Air:");
  u8g.drawStr( 20, 20, "30.99");//dtostrf(t, 5, 2, str));
  u8g.drawStr( 48, 20, "\260C");
  // Humidity
  u8g.drawStr( 64, 20, "Hum:");
  u8g.drawStr( 90, 20, "99.99");//dtostrf(h, 5, 2, str));
  u8g.drawStr( 120, 20, "%");
  // Focuser position
  u8g.drawStr( 0, 30, "Pos:");
  u8g.drawStr( 25, 30, dtostrf(currentPosition, 4, 0, str));
  // Target position
  u8g.drawStr( 64, 30, "Targ:");
  u8g.drawStr( 91, 30, dtostrf(targetPosition, 4, 0, str));
  // Buttons read
  u8g.drawStr( 0, 40, "Analog read:");
  u8g.drawStr( 63, 40, dtostrf(analogRead(buttonPin), 4, 0, str));
  // Motor Status
  if (isMoving) {
    u8g.drawStr( 0, 64, "MOTOR MOVING");
  } else {
    u8g.drawStr( 0, 64, "MOTOR IDLE");
  }
}
