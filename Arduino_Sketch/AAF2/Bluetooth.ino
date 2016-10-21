//-----------------------------------------------------------------------------------------------------------
// HC05 Config
//-----------------------------------------------------------------------------------------------------------
void HC05Config() {
  #ifdef HC05
    BTSerial.begin(9600);                    // switch to 38400 for AT Mode
    delay(500);

    // Put HC05 in AT Command Mode
    digitalWrite(BTSERIAL_PWR_PIN, HIGH);       // Power OFF
    delay(250);
    digitalWrite(BTSERIAL_MODE_PIN, HIGH);      // EN High
    delay(250);
    digitalWrite(BTSERIAL_PWR_PIN, LOW);        // Power ON
  
    delay(2000);

    // Configure Parameters
    BTSerial.print("AT+NAME="+BT_NAME+"\n\r");      // Set Module Name
    delay(2000);
    BTSerial.print("AT+PSWD="+BT_PWD+"\n\r");       // Set Module Password
    delay(2000);

    // Take HC05 out of AT mode and into comms mode
    digitalWrite(BTSERIAL_PWR_PIN, HIGH);       // Power OFF
    delay(250);
    digitalWrite(BTSERIAL_MODE_PIN, LOW);       // EN LOW
    delay(250);
    digitalWrite(BTSERIAL_PWR_PIN, LOW);        // Power ON

    BTSerial.begin(9600);                     // switch back to 9600 for normal comms
  #endif
}
//-----------------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------------------------------------
// HC06 (linvor)Config
//-----------------------------------------------------------------------------------------------------------
void HC06Config() {
  #ifdef HC06
  BTSerial.begin(9600);
  // Should respond with OK
  BTSerial.print("AT");
  waitForResponse();

  // Should respond with its version
  BTSerial.print("AT+VERSION");
  waitForResponse();

  // Set pin to 0000
  BTSerial.print("AT+PIN"+BT_PWD);       // Set Module Password
  waitForResponse();

  // Set the name to BT_NAME
  BTSerial.print("AT+NAME");
  BTSerial.print(BT_NAME);
  waitForResponse();

  Serial.println("Done!");
    // Configure Parameters
    /*delay(2000);
    BTSerial.print("AT+NAME");
    BTSerial.print(BT_NAME);      // Set Module Name BT_NAME
    delay(2000);
    BTSerial.print("AT+PIN"+BT_PWD);       // Set Module Password
    delay(2000);*/

  #endif
}

void waitForResponse() {
  delay(1000);
  while (BTSerial.available()) {
    Serial.write(BTSerial.read());
  }
  Serial.write("\n");
}
//-----------------------------------------------------------------------------------------------------------
