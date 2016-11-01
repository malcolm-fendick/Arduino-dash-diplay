/*
SD card .ini file read/write

Based on an example created Nov 2010 by David A. Mellis, modified 9 Apr 2012 by Tom Igoe.

This example code is in the public domain.

*****************************************************************************************
 k-g.ini file is structured as follows:

DaylightSaving=FALSE
HourNotation=24
SpeedUnits=MPH
SpeedoCalFactor=8.5288

The circuit:

SD card attached to Arduino Due SPI bus as follows:

MOSI - SPI pin 4
MISO - SPI pin 1
SCK  - SPI pin 3
CS   - DUE pin 4

SD card needs 5v. Using a 3.3 Vcc will not work.

*/

//***********************************************************************************************

String readKey(String myKey)
{
  myFile = SD.open("k-g.ini");

  while (myFile.available())
  {
    myString = myFile.readString();
  }
  // close the file:
  myFile.close();

  if (myString.indexOf(myKey) < 0)
  {
    Serial.println(myKey + "Key not found.");
    return "error";
  }
  else
  {
    int myKeyStart = myString.indexOf(myKey);
    int mykeyLength = myKey.length();
    String myStringOne = myString.substring(myKeyStart);
    int myKeyLineLength = myStringOne.indexOf(char(13));
    myStringOne = myString.substring(myKeyStart , myKeyStart + myKeyLineLength);
    String keyVal = myString.substring(myKeyStart + mykeyLength + 1, myKeyStart + myKeyLineLength);

    return keyVal;
  }
}

//***********************************************************************************************

void writeKey(String myKey, String myKeyVal)
{

  myFile = SD.open("k-g.ini");

  while (myFile.available())
  {
    myString = myFile.readString();
  }
  // close the file:
  myFile.close();

  if (myString.indexOf(myKey) < 0)
  {
    Serial.println(myKey + "Key not found.");
  }
  else
  {
    int myKeyStart = myString.indexOf(myKey);
    int mykeyLength = myKey.length();
    String myStringOne = myString.substring(myKeyStart);
    int myKeyLineLength = myStringOne.indexOf(char(13));
    myStringOne = myString.substring(0 , myKeyStart);
    myStringOne = myStringOne + myKey + "=" + myKeyVal;
    String myStringTwo = myString.substring(myKeyStart + myKeyLineLength);
    myStringTwo = myStringOne + myStringTwo;

    Serial.println(myStringTwo);

    SD.remove("k-g.ini");
    myFile = SD.open("k-g.ini", FILE_WRITE);

    // if the file opened okay, write to it:
    if (myFile)
    {
      Serial.print("Writing to k-g.ini...");
      myFile.print(myStringTwo);
      // close the file:
      myFile.close();
      Serial.println("done.");
      Serial.println();
    }
    else
    {
      // if the file didn't open, print an error:
      Serial.println("error opening k-g.ini");
    }

  }
}

//***********************************************************************************************

void createNewIniFile()
{
  SD.remove("k-g.ini");
  myFile = SD.open("k-g.ini", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile)
  {
    Serial.print("Writing to k-g.ini...");

    myFile.println("DaylightSaving=FALSE");
    myFile.println("HourNotation=24");
    myFile.println("SpeedUnits=MPH");
    myFile.println("SpeedoCalFactor=8.5288");

    // close the file:
    myFile.close();
    Serial.println("done.");
    Serial.println();
  }
  else
  {
    // if the file didn't open, print an error:
    Serial.println("error opening k-g.ini");
  }
}
//***********************************************************************************************
