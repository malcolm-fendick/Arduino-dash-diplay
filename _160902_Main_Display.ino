
String SpeedoVersion = "Ver. _160622a_Main_Display";

#include <DueTimer.h>

/****************************************************************************
   Fonts
 ****************************************************************************
*/
#include <Arial14.h>
#include <Arial40Numbers.h>
#include <ArialRounded24.h>
#include <Arial_32x22_Numbers.h>
#include <Arial_Black_16 - Copy (2).h>
#include <Arial_Black_16 - Copy.h>
#include <Arial_Black_16.h>
#include <Droid_Sans_12.h>
#include <Droid_Sans_128.h>
#include <Droid_Sans_16 - Copy.h>
#include <Droid_Sans_16.h>
#include <Droid_Sans_24.h>
#include <Droid_Sans_36 - CopyBak.h>
#include <Droid_Sans_36.h>
#include <Droid_Sans_36_New.h>
#include <Droid_Sans_64.h>
#include <Droid_Sans_96.h>
#include <SystemFont5x7.h>
#include <Trebuchet40.h>
#include <VAGRounded_Thin16x21.h>
#include <VAGROUNDED_Thin38x50.h>
#include <VAGROUNDED_Thin48x62.h>
#include <VAGROUNDED_Thin_50.h>
//#include <fonts/Arial_32x22.h>
#include <Droid_Sans_16.h>
#include <Arial_Black_16.h>
#include <degC_1.h>
#include <Arial_15_mod.h>
#include <Arial_15.h>
#include <Arial_15_let.h>

/****************************************************************************
   Bitmaps
 ****************************************************************************
*/
//#include <RightIndicator16x30x37.h>
//#include <LeftIndicator16x30x37.h>
//#include <K_G_Logo_100x43Neg.h>
//#include <HighBeam16x37x37>

/****************************************************************************
   Pin assignments
 ****************************************************************************
*/

const byte pin_reset = 2;
const byte pin_dc = 3;
const byte pin_sd_cs = 4;
const byte pin_cs = 5;
const byte leftIndicatorPin = 6;
const byte rightIndicatorPin = 7;
const byte highBeamIndicatorPin = 8;
const byte oilWarningPin = 9;
const byte SSPin = 10;               // Reserved as output for sd card
const byte chargeWarningPin = 11;
const byte speedoPin = 12;
const byte ledPin = 13;              // LED test pin
const byte tx = 16;                  // BlueTooth
const byte rx = 17;                  // BlueTooth
const byte txPin = 18;               // GPS
const byte rxPin = 19;               // GPS
const int ONE_WIRE_BUS = 24;         // Dallas Temperature

/****************************************************************************
   Display States
 ****************************************************************************
*/
boolean leftIndOn;
boolean rightIndOn;
boolean highBeamIndOn;
boolean oilWarningOn;
boolean chargeWarningOn;

/****************************************************************************
   BlueTooth
 ****************************************************************************
*/

char inSerial[15];

/****************************************************************************
   Speedo
 ****************************************************************************
*/

//float calibrationFactor = 8.5288;      // pulse edges per metre

float calibrationFactor;
float metres_per_sec;
int km_per_hr;
int miles_per_hr;
int oldSpeed = 1;
int speed_diff;
String speedStr;
String speed_units_Str;

volatile byte pulse_edgeCount;
volatile float pulse_edgeRate;
volatile float old_pulse_edgeRate;
volatile float instant_pulse_edgeRate;

volatile unsigned long holdTime;
volatile unsigned long speedoLoopTime;
volatile unsigned long elapsedTime;
volatile unsigned long intervalTime[16];
volatile boolean startCount;

/****************************************************************************
   FTOLED
 ****************************************************************************
*/

#include <SPI.h>
#include <SD.h>
#include <FTOLED.h>
#include <FTOLED_Colours.h>
#include <progmem_compat.h>

String hourStr;
String minuteStr;
String short_timeStr;

int speed_100s;
int speed_10s;
int old_speed_100s;
int old_speed_10s;
int speed_1s;
int speed_;

OLED oled(pin_cs, pin_dc, pin_reset);

// OLED_TextBox(OLED, Left, Bottom, Width, Height)

//OLED_TextBox speedbox(oled, 14, 52, 75, 32);
//OLED_TextBox speed100sbox(oled, 14, 52, 22, 32);
//OLED_TextBox speed10sbox(oled, 40, 52, 22, 32);
//OLED_TextBox speed1sbox(oled, 66, 52, 22, 32);
OLED_TextBox speedunits(oled, 80, 50, 100, 18);


// OLED_TextBox speedunits(oled, 100, 67, 28, 18);
OLED_TextBox temp(oled, 35, 24, 120, 19);
//OLED_TextBox degCbox(oled, 91, 24, 35, 19);
OLED_TextBox timebox(oled, 0, 0, 59, 21);

OLED_TextBox daybox(oled, 65, 0, 25, 21);
OLED_TextBox monthbox(oled, 91, 0, 37, 15);
OLED_TextBox BTMessage(oled, 0, 0, 128, 80);

#include <Wire.h>

#include <Time.h>
#include <TimeLib.h>

// float metres_per_sec;
// int km_per_hr;
// int miles_per_hr;


/****************************************************************************
   GPS
 ****************************************************************************
*/

boolean GPSFix;
boolean DaylightSaving;
boolean _24hrClock;
boolean mph_kph;
byte options = B00000000;

char charInput [32];
String timeStr;
String dateStr;

int byteGPS = -1;
char NMEAChar[80] = "";
char speedChar[10] = "";
double knot, speedvar;
char strvar[32];
int counta = 0;
int countb = 0;
int indices[13];
boolean messageOK;

volatile int GPSLoopTime;
volatile boolean GPSTick;

/****************************************************************************
   Date/Time
 ****************************************************************************
*/

int hr;
int min;
int sec;
int dy;
int mnth;
int yr;

int oldDate;
int oldTime;

/****************************************************************************
   SD Card
 ****************************************************************************
*/

#include <SD.h>

File myFile;
File dataFile;
String myString;
char SpeedoCalFactorBuf[7];
char calibuf[8];
float SpeedoCalFactor;
volatile boolean logOn;
//boolean logBegin;

/****************************************************************************
   Temperature
 ****************************************************************************
*/
#include <OneWire.h>
#include <DallasTemperature.h>


float old_tempC;
float tempC_diff;

// Setup a oneWire instance to communicate with any OneWire devices (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);

// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);

// arrays to hold device address
DeviceAddress insideThermometer;

boolean tempPos;
boolean tempAlert;
volatile int TempLoopTime;
volatile int TempReadingInt;

/****************************************************************************
   setup
 ****************************************************************************
*/
void setup()
{
  Serial.begin(115200);
  Serial.println ("Setting up - sketch version " + SpeedoVersion);
  Serial.println ("************************************************");
  Serial.println ();

  /****************************************************************************
     FTOLED setup()
   ****************************************************************************
  */
  Serial.print("Setting up FTOLED..");

  pinMode(leftIndicatorPin, INPUT);
  pinMode(rightIndicatorPin, INPUT);
  pinMode(highBeamIndicatorPin, INPUT);
  pinMode(oilWarningPin, INPUT);
  pinMode(chargeWarningPin, INPUT);


  oled.begin();
  oled.clearScreen();
  oled.setOrientation(ROTATE_180);
  oled.fillScreen(BLACK);
  GPSFix = FALSE;
  // Display logo
  K_G_Logo();
  Serial.print("..");

  speedunits.setForegroundColour(WHITE);
  temp.setForegroundColour(GREY);
  timebox.setForegroundColour(GREY);
  daybox.setForegroundColour(GREY);
  monthbox.setForegroundColour(WHITE);

  Serial.println(".. Done");

  /****************************************************************************
    BlueTooth setup
  ****************************************************************************
  */
  Serial2.begin(9600);
  logOn = FALSE;
  //logBegin = FALSE;
  /****************************************************************************
    setup sd card and read k-g.ini file
  ****************************************************************************
  */

  // Open serial communications and wait for port to open:

  while (!Serial)
  {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  Serial.print("Initializing SD card...");

  /*
    On the Ethernet Shield, CS is pin 4. It's set as an output by default.
    Note that even if it's not used as the CS pin, the hardware SS pin
    (10 on most Arduino boards, 53 on the Mega) must be left as an output
    or the SD library functions will not work.
  */

  pinMode(SSPin, OUTPUT);

  if (!SD.begin(4))
  {
    Serial.println("SD card initialization failed!");
    //return;
  }
  Serial.println("SD card initialization done.");

  /*************************************************************************
    Run this routine if a new, clean or modified k-g.ini file is needed
  *************************************************************************/

  // createNewIniFile();

  //=======================================================================


  /*************************************************************************
    Set Daylight saving
  *************************************************************************/
  String daylightSavingVal = readKey("DaylightSaving");

  if (daylightSavingVal == "TRUE")
  {
    DaylightSaving = TRUE;
  }
  else if (daylightSavingVal == "FALSE")
  {
    DaylightSaving = FALSE;
  }
  /*************************************************************************
    Set 12/24 hour notation
  *************************************************************************/
  String hourNotationVal = readKey("HourNotation");

  if (hourNotationVal == "24")
  {
    _24hrClock = TRUE;
  }
  else if (hourNotationVal == "12")
  {
    _24hrClock = FALSE;
  }
  /*************************************************************************
    Set speed units
  *************************************************************************/
  String speedUnitsVal = readKey("SpeedUnits");

  if (speedUnitsVal == "MPH")
  {
    mph_kph = TRUE;
    speed_units_Str = "mph";
  }
  else if (speedUnitsVal == "KPH")
  {
    mph_kph = FALSE;
    speed_units_Str = "km/h";
  }
  /*************************************************************************
    Set speedo calibration factor
  *************************************************************************/
  String calibrationFactorStr = readKey("SpeedoCalFactor");

  Serial.print("Calibration Factor = ");
  Serial.println(calibrationFactorStr);

  calibrationFactor = calibrationFactorStr.toFloat();

  Serial.print("Calibration Factor = ");
  Serial.println(calibrationFactor, DEC);

  //=========================================================================

  Serial.println("Values from k-g.ini file.");
  Serial.println(daylightSavingVal + ", " + hourNotationVal + ", " + speedUnitsVal + ", " + calibrationFactor);

  /*************************************************************************
    Run these routines if diagnotics needed
  *************************************************************************/

  //  writeKey("HourNotation", "12");
  //  writeKey("DaylightSaving", "TRUE");
  //  writeKey("SpeedoCalFactor", "5.3369587");
  //  writeKey("SpeedUnits", "KPH");

  /****************************************************************************
    Speedo setup()
  ****************************************************************************/

  // Initialize a serial connection for reporting values to the host
  //  Serial.begin(115200);
  Serial.print("NewSpeedo with Due ");
  Serial.println(SpeedoVersion);

  pinMode(speedoPin, INPUT);
  //digitalWrite(speedoPin, HIGH);   // needed to set internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(speedoPin), Speedo_pulse, RISING);

  startCount = true;

  /****************************************************************************
    GPS setup()
  ****************************************************************************/

  pinMode(ledPin, OUTPUT);       // Initialize LED pin
  digitalWrite(ledPin, HIGH);

  Serial1.begin(115200);

  Serial.println("Starting ...");

  pinMode(rxPin, INPUT);
  pinMode(txPin, OUTPUT);

  for (int i = 0; i < 80; i++)
  { // Initialize a buffer for received data
    NMEAChar[i] = '\0';
  }
  Serial.println(".. Done");
  /****************************************************************************
      Temperature setup()
    ****************************************************************************
  */
  Serial.println("Dallas Temperature IC Control Library");

  // locate devices on the bus
  Serial.print("Locating devices...");
  sensors.begin();
  Serial.print("Found ");
  Serial.print(sensors.getDeviceCount(), DEC);
  Serial.println(" devices.");

  // report parasite power requirements
  Serial.print("Parasite power is: ");
  if (sensors.isParasitePowerMode()) Serial.println("ON");
  else Serial.println("OFF");

  if (!sensors.getAddress(insideThermometer, 0)) Serial.println("Unable to find address for Device 0");

  // show the addresses we found on the bus
  Serial.print("Device 0 Address: ");
  // printAddress(insideThermometer);
  Serial.println();

  // set the resolution to 9 bit (Each Dallas/Maxim device is capable of several different resolutions)
  sensors.setResolution(insideThermometer, 10);

  Serial.print("Device 0 Resolution: ");
  Serial.print(sensors.getResolution(insideThermometer), DEC);
  Serial.println();

  /****************************************************************************
    Timers setup()
  *****************************************************************************
  */

  // Timer3.attachInterrupt(leftIndicator).start(600000); // Every 600ms

  // Timer.getAvailable().attachInterrupt(leftIndicator).start(600000);

  // Start timer on first available timer

  // DueTimer::getAvailable().attachInterrupt(callback2).start(10);
  // Start timer on second available timer
  // And so on...

  /****************************************************************************
    Logging setup()
  *****************************************************************************
  */
  logOn = FALSE;
  oled.fillScreen(BLACK);
  oled.selectFont(Arial_Black_16);
  speedunits.print(speed_units_Str);
}

//****************************************************************************

void loop()

{
  BlueTooth();

  scanGPS();

  showWarnings();

  // Speedo() is run every 500 ms, syncronised with the 1 Hz GPS signal
  if (GPSTick == TRUE)
  {
    Speedo();
    getTemperature();
  }
  else if ((millis() - speedoLoopTime) > 500)
  {
    Speedo();
    getTemperature();
  }
}

//****************************************************************************
