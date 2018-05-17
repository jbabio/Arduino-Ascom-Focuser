void draw() {
  // Telescope tube temperature
  u8g2.setFont(u8g2_font_freedoomr10_tu);  // choose a suitable font
  u8g2.drawStr( 2, 15, "T1:");
  u8g2.drawStr( 24, 15, dtostrf(currentTemperature, 5, 2, str));
  //u8g2.drawStr( 58, 12, "\260C");
  // Air temperature
  u8g2.drawStr( 66, 15, "T2:");
  u8g2.drawStr( 88, 15, dtostrf(temp, 5, 2, str));
  //u8g2.drawStr( 48, 23, "\260C");
  // Humidity
  u8g2.drawStr( 2, 31, "RH:");
  u8g2.drawStr( 24, 31, dtostrf(hum, 5, 2, str));
  //u8g2.drawStr( 120, 23, "%");
  // Dew Point
  u8g2.drawStr( 66, 31, "DP:");
  u8g2.drawStr( 88, 31, dtostrf(dewPoint, 5, 2, str));
  // Pressure
  u8g2.drawStr( 2, 47, "PS:");
  u8g2.drawStr( 24, 47, dtostrf(( pres / 100), 5, 2, str));


  // Motor Status
  if (isMoving) {
    u8g2.drawFrame(0, 48, 127, 16);                             // Draw "Moving" frame
    u8g2.drawStr( 2, 63, "MOVING TO:");
    u8g2.drawStr( 80, 63, dtostrf(targetPosition, 4, 0, str));
  } else {
    u8g2.drawFrame(0, 48, 97, 16);                             // Draw "Idle" frame
    u8g2.drawFrame(97, 48, 31, 16);                            // Draw Step multiplier frame
    // Step size
    switch (focusStepSize) {
      case 1:
        u8g2.drawStr( 102, 63, "M:I");  //1
        break;
      case 10:
        u8g2.drawStr( 102, 63, "M:X");  //10
        break;
      case 100:
        u8g2.drawStr( 102, 63, "M:C");  //100
        break;
      case 1000:
        u8g2.drawStr( 102, 63, "M:M");  //1000
        break;
    }
    u8g2.drawStr( 0, 63, "IDLE ON:");
    u8g2.drawStr( 61, 63, dtostrf(currentPosition, 4, 0, str));
  }
}

void bootingBme280() {
  u8g2.setFont(u8g2_font_freedoomr10_tu);  // choose a suitable font
  u8g2.drawStr( 0, 12, "BOOTING BME280");
}

void drawDataFrames() {
  //First line
  u8g2.drawFrame(0, 0, 64, 16);
  u8g2.drawFrame(64, 0, 64, 16);
  //Second line
  u8g2.drawFrame(0, 16, 64, 16);
  u8g2.drawFrame(64, 16, 64, 16);
  //Third line
  u8g2.drawFrame(0, 32, 79, 16);
  u8g2.drawFrame(79, 32, 68, 16);
  u8g2.drawFrame(117, 32, 11, 16);
  //Yellow section

}

