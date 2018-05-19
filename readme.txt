AAF2JB Release 1.0.6
======================
This project is a derivative from AAF2 by @tekkydave (https://sourceforge.net/projects/arduinoascomfocuser/?source=navbar)

It requires the Ascom platform 6.1 to be installed first from the Ascom website (http://ascom-standards.org/)

The 3 project components are:

1) A windows installer program (AAF2Setup.msi)

It will install: 
- The Ascom driver for the focuser. This is a windows COM object which should be registered by the installer. 
- A command-line test program, AAF2Test that just reports the focuser version (#V command) from the arduino. 
- A GUI client program, FocusAAF2 that allows full control of the focuser. This is also a work in progress. It is intended for testing
  and is not designed to be a full-blown focuser application. There are many fine fully-featured applications that can use this driver.

Note: If you have a previous version of AAF2 installed then uninstall it first before installing the latest version.

2) An Arduino sketch (AAF2JB_X.X.X.ino)
- This needs to be uploaded to the Arduino in your focuser.

3) Two Arduino Libraries that support the 1-wire and Temperature Sensor functions used by the sketch. 
   You will need to install these in order to compile and upload the sketch even if you don't intend to connect a temperature sensor.
   The libraries are in the Files section of this site along with the other files.
   - OneWire.zip
   - DallasTemperature.zip

   Note that the focuser will still work without the temperature sensor attached but it will always give a temperature reading of 0C.

   If you are unsure about how to install Libraries see the instructions here: http://arduino.cc/en/Guide/Libraries
   Under the section 'Automatic installation' there are clear instructions on how to install the library zip files via the Arduino IDE.
   Install the two libraries before trying to Compile & Upload the sketch to your Arduino device.

HARDWARE
--------

------------------------                              | -----------------------------
jD-IOBoard V1.0 pinouts                               | BME280 Sensor       ->  Board
------------------------                              | -----------------------------
IMPORTANT: NEWER JD_IOBoard REVISION CHANGED PINOUT   | Vin (Voltage In)    ->  3.3V/5V
           READ io-board_v1.1.pdf for more detail.    | Gnd (Ground)        ->  Gnd
            D D D G       R T R                       | SDA (Serial Data)   ->  A4
        5 5 1 1 1 N D D D X X S                       | SCK (Serial Clock)  ->  A5
        V V 3 1 2 D 7 6 5 1 1 T                       | 
        | | | | | | | | | | | |                       | -----------------------------
     +----------------------------+                   | SSD1306 OLED Screen ->  Board
     |O O O O O O O O O O O O O   |                   | -----------------------------
O1 - |O O   S M M                O| _ DTS             | Vin (Voltage In)    ->  3.3V/5V
O2 - |O O   C O I                O| - RX  F           | Gnd (Ground)        ->  Gnd
O3 - |O O   K S S       S S      O| - TX  T           | SDA (Serial Data)   ->  A4
O4 - |O O     I O       D C      O| - 5V  D           | SCK (Serial Clock)  ->  A5
O5 - |O O               A L      O| _ CTS I           | 
O6 - |O O O O O O O O   O O O O  O| - GND             | -----------------------------
     +----------------------------+                   | XDeSIG Keypad   ->  Board
      |   | | | | | |   | | | |                       | -----------------------------
      C   G 5 A A A A   A A 5 G                       | Vin (Voltage In)    ->  3.3V/5V
      O   N V 0 1 2 3   4 5 V N                       | Signal (v. divider) ->  A3
      M   D                   D                       | Gnd (Ground)        ->  Gnd
	  

Changes in Version JB 1.0.6   15/05/2018  
----------------------------------------
Dropped U8glib, U8g2 from the same author will be used onwards. U8g2 usesthe standard Wire.h library thus allowing
the use of the BME280 i2c sensor.

	Repository: https://github.com/olikraus/u8g2
	Reference:  https://github.com/olikraus/u8g2/wiki/u8g2reference

Since fonts can use huge amounts of program memory, the small footprint fontu8g2_font_freedoomr10_tu (658 Bytes)
was selected. This means that only uppercase letters, numbers and basic mathematic simbols are available.
                      
BME280 is now suppoted as ambient sensor via BME280 Library from Finitespace, this sensor can sense temperature,
humidity and atmosferic pressure.

	Repository: https://github.com/finitespace/BME280

To save screen space, the manual step size is now shown in roman numbers: 1 -> I | 10 -> X | 100 -> C | 1000 -> M

Removed a delay() on getAmbientData(), it was there for debug purposes.

Display layout revamped.
                      
Known issue: Currently, the ascom driver won't be able to handle new ambient data, so that information
will be shown on screen only. 

Changes in Version JB 1.0.5   22/04/2018 
----------------------------------------
Center button turns on/off the screen.

Manual step size shown on screem.

Changes in Version JB 1.0.4   20/04/2018 
----------------------------------------
Added 128x64 OLED display output through i2c with U8glib library.

	Repository: https://github.com/olikraus/u8glib
	Reference:  https://github.com/olikraus/u8glib/wiki/userreference
                      
Added a voltage divider based keypad with 5 keys which uses only one analog pin by XDeSIG.
                      
	https://www.pcbway.com/project/shareproject/W50475ASN5_Escorna_bot_2_1_Gerber.html
                      
Known issue: Currently, the ascom driver can't be notified automatically of manual position changes, to override 
this and sync the ascom client with the focuser, you'll have to press halt/stop buton wich triggers a resync to
current position. 

Changes in Version JB 1.0.3   17/04/2018
----------------------------------------
Modified Clockwise(), AntiClockwise() and related functions to be non blocking.

Changes in Version JB 1.0.2   16/04/2018  
----------------------------------------
Replaced all #define with const

Changes in Version JB 1.0.1   15/04/2018  
----------------------------------------
Switched from multiple board system (arduino Nano+HC05+ULN2003)to a JD_IOBoard V1.0 which include an Atmega+ULN2003
in the size of an Arduino pro mini.

Droped BT functionality and its buttons. 

Changes in Version JB 1.0.0   20/10/2016
----------------------------------------
Changed name and version number to avoid confusion with original AAF2. Name will be AAF2JB and version starts at 1.0.0

Removed the #define BLUETOOTH_COMMS method to activate BT since that "harcoded" method makes imposible to activate and 
deactivate the BT radio without editing the code and reuploading it to the board.

Two buttons added (Pins 11 & 12). One to allow turning BT on/off and the other to perform BT configuration.

Functions were moved to different tabs depending on their purpose.

Multiple code modifications.

**FORK**FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK****FORK**

Changes in Version 2.5.0
------------------------
This version integrates a HC05 Bluetooth module. This provides remote connection via Bluetooth and therefore the focuser does not need a
permanent USB cable attached. Only the Arduino sketch has been changed.
This sketch is backward compatible with the current Ascom driver and is supplied with the Bluetooth option turned OFF.
                                                                                          ~~~~~~~~~~~~~~~~~~~~~~~~~~~~
To enable it uncomment the line

//#define BLUETOOTH_COMMS                                   // Use Bluetooth Comms - if commented out defaults to USB Serial Port

in the sketch prior to uploading. You will need to connect your HC05 as per the updated diagram, AAF2_V2.5.0_crop.png in the Files area.
If you use different pins on your Arduino then update the following lines in the sketch accordingly:

  #define BTSERIAL_PWR_PIN                2                // Bluetooth serial Power Pin
  #define BTSERIAL_MODE_PIN               3                // Bluetooth serial Mode Pin
  #define BTSERIAL_RX_PIN                 4                // Bluetooth serial RX Pin
  #define BTSERIAL_TX_PIN                 5                // Bluetooth serial TX Pin

The HC05 is configured automatically by the Arduino at start-up. This assumes it is already set to the slave mode which it will be by default.
The default device name and password are set to 'Focuser' and '9999' but can be changed by amending these lines:

  const String BT_NAME = "Focuser";                        // Bluetooth Device Name
  const String BT_PWD  = "9999";                           // Bluetooth Device Password (Pairing Code)


Changes in Version 2.4.0
------------------------
Enhancements:
* The Arduino sketch has the following changes:
   - Amended to step the motor in single half-steps not 8 per unit. (One unit is a single step from the driver/application's point of view).
     The number of half-steps to execute per unit of movement is set in #define MOTOR_STEPS_PER_DRIVER_STEP
	 Set this to 8 (default) to keep the existing behaviour. If you want finer movement lower it as required (may need some experimentation).

   - Moved user-configurable parameters to a single block at top of code for easier location / modification
   - Reduced number of read/write operations to/from EEPROM. This should lengthen the life of the EEPROM storage.
   - Reduced no of calls to clear the output pins in loop()
   - Some general code and structure tidying

* The Ascom Driver has the following changes:
   - Driver Registration should now work correctly (woohoo). The Setup project has had the correct Registry entries added.

   - The trace files are now created in the AAF2Trace directory which is a sub-directory of the user's Personal Documents Folder.
     Normally this will be at C:\Users\<Username>\Documents but may be different if your system is non-standard. There is usually a
	 shortcut in Windows Explorer to 'My Documents' which is the same location. The AAF2Trace sub-directory is created automatically
	 during installation so there is no longer a need to create it manually. The location is stored in the Registry so the Driver can
	 access it.

   - The 'Trace on' option in the Ascom Chooser dialog is now unset automatically when the Driver exits. If you need to trace the Driver during
     normal use you will need to tick it every time you use the Driver. This is to save the disk filling up if you forget to turn it off.


Changes in Version 2.3.0
------------------------
Enhancements:
* The Arduino now saves it's current position in EEPROM (non-volatile memory). It will remember the position it was at between
  focuser and client application restarts / disconnects. The chooser dialog has been modified to enable you to override the initial
  starting position if required.
* Some users in countries other than the UK reported issues with the temperature not being interpreted correctly due to the use
  of a dot as the decimal separator. This has been removed and the Arduino now returns the temperature in 100ths of a degree (no decimal).
  The driver has been updated to divide by 100 prior to returning the value to the calling client program. Windows should put
  the correct decimal separator in the value according to the PC's international settings.
  I have also blocked the reading of the 1-wire temperature sensor whilst the motor is moving as it interferes with smooth movement.
  During movement it will return the last read value. When the motor is not moving it will re-read the sensor. This is because many
  client applications seem to read it every second which seems excessive. The FocusAAF2 test program now also reads every second for testing.

Bug fixes:
* Changed some int variables to unsigned int
* Fixed a minor bug in the motor hi/lo speed detection - it was only working in one direction.

Changes in Version 2.2.0
------------------------
Implemented Temperature Sensing.
Arduino Sketch - Implemented C# command. Uses 1-wire DS18B20. New functionality requires two Libraries.
  Ascom Driver - Implemented Ascom Temperature property.

Changes in Version 2.1.3
------------------------
Implemented the Ascom Halt function.
Added new Arduino sketch command Innnn#. This allows an initial position nnnn to be set. A textbox has been added to the
chooser properties so initial position can be set by user. Position is remembered between disconnect/connect.

Changes in Version 2.1.2
------------------------
Changes to prevent negative positions:
Arduino Sketch - This has been updated to have a start position of 1000.
  Ascom Driver - This has been amended so negative positions cannot be achieved.

Changes in Version 2.1.1
------------------------
Hopefully I have fixed the issue with the COM object not being automatically registered.

Changes in Version 2.1.0
------------------------
Changes to look of FocusAAF2 test program:
	- Buttons now display the number of steps they action up/down.
	- Some colour added to make it less boring.

Version 2.0.0
-------------
Initial Release
