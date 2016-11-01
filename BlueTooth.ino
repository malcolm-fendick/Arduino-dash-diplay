void BlueTooth()
{
  int i = 0;
  int m = 0;
  String messageStr = "";

  if (Serial2.available() > 0)
  {
    while (Serial2.available() > 0)
    {
      inSerial[i] = Serial2.read();
      delay (5);
      i++;
    }
    inSerial[i] = '\0';
    messageStr = String(inSerial);

    Serial.println(messageStr);

    oled.selectFont(Droid_Sans_12);
    oled.clearScreen();

    //======================================================
    if (messageStr == "logon")
    {
      BTMessage.println("Logging on");
      Serial.println("log started");
      logOn = TRUE;
      logData("Beginning log:");
      timebox.setBackgroundColour(RED);
    }
    else if (messageStr == "logoff")
    {
      BTMessage.println("Logging off");
      Serial.println("log stopped");
      logOn = FALSE;
      logData("Stopping log:");
      timebox.setBackgroundColour(BLACK);
    }
    //======================================================
    else if (messageStr == "bst")
    {
      BTMessage.println("Daylight saving on");
      Serial.println("Daylight saving on");
      DaylightSaving = TRUE;
      writeKey("DaylightSaving", "TRUE");
    }
    else if (messageStr == "gmt")
    {
      BTMessage.println("Daylight saving off");
      Serial.println("Daylight saving off");
      DaylightSaving = FALSE;
      writeKey("DaylightSaving", "FALSE");
    }
    //======================================================
    else if (messageStr == "24h")
    {
      BTMessage.println("24 hour clock");
      Serial.println("24 hour clock");
      _24hrClock = TRUE;
      writeKey("HourNotation", "24");
    }
    else if (messageStr == "12h")
    {
      BTMessage.println("12 hour clock");
      Serial.println("12 hour clock");
      _24hrClock = FALSE;
      writeKey("HourNotation", "12");
    }
    //======================================================
    else if (messageStr == "mph")
    {
      BTMessage.println("mph units");
      Serial.println("mph units");
      mph_kph = TRUE;
      writeKey("SpeedUnits", "MPH");
      speed_units_Str = "mph";
    }
    else if (messageStr == "kph")
    {
      BTMessage.println("kph units");
      Serial.println("kph units");
      mph_kph = FALSE;
      writeKey("SpeedUnits", "KPH");
      speed_units_Str = "km/h";
    }
    //======================================================
    else if (messageStr.startsWith("cal"))
    {
      messageStr = messageStr.substring(3);
      messageStr.toCharArray(calibuf, 8);
      calibrationFactor = atof(calibuf);
      BTMessage.println(calibrationFactor, DEC);
      Serial.println(calibrationFactor, DEC);
      writeKey("SpeedoCalFactor", messageStr);
    }
    //======================================================
    else
    {
      BTMessage.println("Not recognised");
      Serial.println("Not recognised");
    }


    delay (5000);

    messageStr = "";
    BTMessage.clear();
    oled.clearScreen();
    oldTime = 0;
    oldDate = 0;
    oldSpeed = 1;
    oled.selectFont(Arial_Black_16);
    speedunits.clear();
    speedunits.print(speed_units_Str);

  }
}
