//----------------------------------------------------------------------------------------------------------------------
// Include necessary libraries
//----------------------------------------------------------------------------------------------------------------------
#include <OneWire.h>                                     // DS18B20 temp sensor
#include <DallasTemperature.h>                           // DS18B20 temp sensor  
#include <EEPROM.h>                                      // EEPROM Library
#include <SoftwareSerial.h>                              // For Bluetooth Connection
//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// User-configurable values. Only change values if you know what you are doing.
//----------------------------------------------------------------------------------------------------------------------
  //----------------
  // PIN OUT
  //----------------
    #define BUTTON1_PIN                     11           // Button 1 (BT or USB)
    #define BUTTON2_PIN                     12           // Button 2 (TBD)
    #define BTSERIAL_PWR_PIN                2            // Bluetooth serial Power Pin
    #define BTSERIAL_MODE_PIN               3            // Bluetooth serial Mode Pin
    #define BTSERIAL_RX_PIN                 4            // Bluetooth serial RX Pin
    #define BTSERIAL_TX_PIN                 5            // Bluetooth serial TX Pin
    #define MOTOR_PINS {7,8,9,10};                       // Declare pins to drive motor control board
    #define ONE_WIRE_BUS                    6            // DS18B20 DATA wire connected to Digital Pin 6
    #define USBLED_PIN                      13           // USB-mode led pin

  //----------------
  // Stepper Motor  
  //----------------
    #define MOTOR_STEPS_PER_DRIVER_STEP     8            // Motor steps per Ascom movement Unit (old method = 8)
    #define MOTOR_SPEED_DEFAULT             16000        // Default motor step speed (uS)(failsafe operation)
    #define MOTOR_SPEED_LO                  16000        // Motor step delay for Lo speed (uS)
    #define MOTOR_SPEED_HI                  2000         // Motor step delay for Hi speed (uS)
    #define MOTOR_SPEED_THRESHOLD           100          // motor speed Hi if steps to go is higher than this
    #define DEFAULT_START_POSITION          5000         // Default Start Position if not set by Innnn# command
    
  //----------------
  // Temperature    
  //----------------
    #define TEMP_RESOLUTION                 10           // 1-wire temperature sensor resolution 9=9bit(0.50C), 10=10bit(0.25C), 11=11bit(0.125C), 12=12bit(0.0625C)

  //---------------------------
  // EEPROM storage parameters 
  //---------------------------
    #define EE_LOC_POS 0                                 // Location of position (2 bytes)
    #define EE_LOC_PSTAT 2                               // Location of Position Status (1 Byte)
    #define POS_VALID 55                                 // Stored position valid if this value otherwise invalid

//----------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------
// Variables
//----------------------------------------------------------------------------------------------------------------------

const String programName = "Arduino Focuser";
const String programVersion = "1.0.0";
const String BT_NAME = "AAF2JB";                           // Bluetooth Device Name
const String BT_PWD  = "0000";                             // Bluetooth Device Password (Pairing Code)

const int motorPins[4] = MOTOR_PINS;                       // Declare pins to drive motor control board

int           motorSpeed = MOTOR_SPEED_DEFAULT;            // current delay for motor step speed (uS)
DeviceAddress tempSensor;                                  // Temperature sensor
double        currentTemperature;                          // current temperature
boolean       tempSensorPresent = false;                   // Is there a temperature sensor installed?
int           step = 0;                                    // current motor step position
boolean       outputActive = true;                         // Is motor energised? Initialised to true so it gets cleared on startup

// Default initial positions if not set using the Innnn# command by Ascom Driver
unsigned int  currentPosition = DEFAULT_START_POSITION;    // current position
unsigned int  targetPosition  = DEFAULT_START_POSITION;    // target position
unsigned int  lastSavedPosition;                           // last position saved to EEPROM

// Initialise the temp sensor
OneWire oneWire(ONE_WIRE_BUS);                             // Setup a oneWire instance to communicate with any OneWire devices
DallasTemperature sensors(&oneWire);                       // Pass our oneWire reference to Dallas Temperature.

// Lookup table to drive motor control board
const int stepPattern[8] = {B01000, B01100, B00100, B00110, B00010, B00011, B00001, B01001};

// For ASCOM connection
String inputString = "";                                   // string to hold incoming data
boolean stringComplete = false;                            // whether the string is complete
boolean isMoving = false;                                  // is the motor currently moving

// Software serial
  SoftwareSerial BTSerial(BTSERIAL_RX_PIN, BTSERIAL_TX_PIN);  // SoftwareSerial Serial Port for Bluetooth module

// For Butons
int button1State = 0;
int button2State = 0;

//function prototypes
void HC05Config();
void HC06Config();
void waitForResponse();
void doMovement();
void anticlockwise();
void clockwise();
void setOutput(int out);
void clearOutput();
void savePosition(unsigned int p);
unsigned int restorePosition(void);
boolean storedPositionValid(void);
void serialEvent();
void BTSerialEvent();
void doCommand(String command);
void doCommandBT(String command);
double getTemperature();
// end function prototypes

//=============================================================================================================
// Setup
//=============================================================================================================

void setup(){
  //-----------------------------------------------------------------------------------------------------------
  // Setup IO Pins
  //-----------------------------------------------------------------------------------------------------------

  // Motor Pins
  for (int i = 0; i < 4; i++) {
    pinMode(motorPins[i], OUTPUT);
    digitalWrite(motorPins[i], 0);                       // Ensure all motor coils de-energised
  }
  outputActive = false;
  
  // Bluetooth
  pinMode(BTSERIAL_MODE_PIN, OUTPUT);
  digitalWrite(BTSERIAL_MODE_PIN, LOW);
  pinMode(BTSERIAL_PWR_PIN, OUTPUT);                      //Initialize BTSERIAL_PWR_PIN as output
  digitalWrite(BTSERIAL_PWR_PIN, HIGH);                   //Make sure BT is powered off
  
  // LEDs
  pinMode(USBLED_PIN, OUTPUT);                            //Initialize LED pin as output
  
  //Buttons
  pinMode(BUTTON1_PIN, INPUT);                            //BUTTON1_PIN as input
  pinMode(BUTTON2_PIN, INPUT);                            //BUTTON2_PIN as input
  
//-----------------------------------------------------------------------------------------------------------
// Check SetUp switches And initialization
//-----------------------------------------------------------------------------------------------------------
  inputString.reserve(200);                          // reserve 200 bytes for the ASCOM driver inputString
  button1State = digitalRead(BUTTON1_PIN);           //read the state of BUTTON1_PIN:
  if (button1State == HIGH){
    digitalWrite(BTSERIAL_PWR_PIN, LOW);             // Turn ON BT module
    digitalWrite(USBLED_PIN, LOW);                   // Turn OFF USB-Mode LED
    BTSerial.begin(9600);                            // Initialize BT serial for ASCOM interface
    //#define HC06                                   // Set Up HC05 or HC06 (linvor)
  }else{
    digitalWrite(USBLED_PIN, HIGH);                  // Turn ON USB-Mode LED
    Serial.begin(9600);                              // Initialize USB serial for ASCOM interface
  }
//-----------------------------------------------------------------------------------------------------------
// HC05/06 Config if required
//-----------------------------------------------------------------------------------------------------------
  /*#ifdef HC05
  HC05Config();
  #endif
  #ifdef HC06
  HC06Config();
  #endif*/
  //-----------------------------------------------------------------------------------------------------------
  // Position initialisation
  //-----------------------------------------------------------------------------------------------------------
  if (storedPositionValid()) {                       // Check if EEPROM position is valid
    currentPosition = restorePosition();             // Use position from EEPROM if it is valid
  } else {
    currentPosition = DEFAULT_START_POSITION;        // If invalid use the default position
  }
  lastSavedPosition = currentPosition;
  targetPosition = currentPosition;
  //-----------------------------------------------------------------------------------------------------------
  // OneWire Libary initialisation
  //-----------------------------------------------------------------------------------------------------------
  oneWire.reset_search();                            // Reset search
  oneWire.search(tempSensor);                        // Search for temp sensor and assign address to tempSensor
  //-----------------------------------------------------------------------------------------------------------
  // DallasTemperature Library initialisation
  //-----------------------------------------------------------------------------------------------------------
  sensors.begin();                                   // Initialise 1-wire bus
  //-----------------------------------------------------------------------------------------------------------
  // Temperature sensor initialisation
  //-----------------------------------------------------------------------------------------------------------
  if (sensors.getDeviceCount() == 0) {
    tempSensorPresent = false;                       // temperature sensor not installed
  } else {
    tempSensorPresent = true;                               // temperature sensor installed - set it up and get initial value
    sensors.setResolution(tempSensor, TEMP_RESOLUTION);     // Set the resolution
    sensors.requestTemperatures();                          // Get the Temperatures
    currentTemperature = getTemperature();                  // Save current temperature
  }
    //clearOutput();                                     // Ensure all motor coils de-energised
 //-----------------------------------------------------------------------------------------------------------
}
//=============================================================================================================
// Main Loop
//=============================================================================================================

void loop(){
  // If using Bluetooth call doSerialEvent explicitly to check for incoming commands
  // If using USB Serial it has built-in serialEvent which is called automatically between every loop()
  if (button1State == HIGH){           //Check if BT mode is active.
    BTSerialEvent();
    if (stringComplete){
      doCommandBT(inputString);
    }
  }else{

  // Process the command string if complete
  if (stringComplete){
    doCommand(inputString);
  }
  }
  // If any motor movement required do it
  doMovement();
  }
//=============================================================================================================


