/*

  GPS set @ 1 Hz sampling, $GPRMC only and 115200 serial speed in FLASH memory.
  Venus GPS connected to Due Serial1, TX1 pin 18, RX1 pin 19.

  $GPRMC
  Recommended minimum specific GPS/Transit data

  eg. $GPRMC,220516,A,5133.82,N,00042.24,W,173.8,231.8,130694,004.2,W*70
              1    2    3    4    5     6    7    8      9     10  11 12

      1   220516     Time Stamp
      2   A          validity - A-ok, V-invalid
      3   5133.82    current Latitude
      4   N          North/South
      5   00042.24   current Longitude
      6   W          East/West
      7   173.8      Speed in knots
      8   231.8      True course
      9   130694     Date Stamp
      10  004.2      Variation
      11  W          East/West
      12  *70        checksum

  Based on example code for connecting a Parallax GPS module to the Arduino by
  Igor Gonzalez Martin. 05-04-2007 - igor.gonzalez.martin@gmail.com
  English translation by djmatic 19-05-2007

*/

void scanGPS()
{
  digitalWrite(ledPin, HIGH);

  messageOK = false;
  byteGPS = Serial1.read();       // Scanning bytes into the serial port

  if (byteGPS == 36)             // look for the start of the NMEA message ($)
  {
    //Serial.println("Start of NMEA message");

    for (int i = 0; i < 80; i++) // resets ready for next message
    {
      NMEAChar[i] = '\0';
    }

    NMEAChar[0] = '$';
    counta = 1;

    while (counta < 80)
    {
      byteGPS = Serial1.read();       // Read a byte of the serial port
      if (byteGPS == -1)              // See if the port is empty
      {
        delay(1);
      }
      else
      {
        NMEAChar[counta] = byteGPS;

        if (NMEAChar[counta] == 42)  // stops when "*" is found
        {
          counta = 79;
          messageOK = true;
          digitalWrite(ledPin, LOW);
        }
        counta++;
      }
    }
    for (int i = 0; i < 80; i++)
    {
      Serial.print(NMEAChar[i]);
    }
    Serial.println();
    messageOK = true;
  }

  if (messageOK == true)
  {
    countb = 0;

    for (int i = 0; i < 80; i++)
    {
      if (NMEAChar[i] == ',')
      { // check for the position of the  "," separators
        indices[countb] = i;
        countb++;
      }
    }

    if (NMEAChar[indices[1] + 1] != 65)
    {
      Serial.print(" *** No fix *** ");
      Serial.println();
    }

    else
    {
      if (GPSFix == FALSE)
      {
        // oled.fillScreen(BLACK);
        // oled.selectFont(Arial_Black_16);
        //speedunits.print("mph");
        GPSFix = TRUE;
      }

      for (int i = 0; i < 12; i++)
      {
        switch (i)
        {
          // ----------------------------------------------------------------------------------------
          case 0 :                                // Retrieves time
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            char timeChar[3];

            timeChar[0] = NMEAChar[indices[i] + 1];
            timeChar[1] = NMEAChar[indices[i] + 2];

            hr = atoi( timeChar );

            timeChar[0] = NMEAChar[indices[i] + 3];
            timeChar[1] = NMEAChar[indices[i] + 4];

            min = atoi( timeChar );

            timeChar[0] = NMEAChar[indices[i] + 5];
            timeChar[1] = NMEAChar[indices[i] + 6];

            sec = atoi( timeChar );

            //timeStr = String(hr) + ":" + String(min) + ":" + String(sec);

            break;

          // ----------------------------------------------------------------------------------------
          case 6 :                               // Retrieves speed

            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            for (int j = 0; j < 32; j++)
            {
              charInput[j] = '\0';                // Resets these two string variables
              strvar[j] = '\0';
            }

            for (int j = indices[i]; j < (indices[i + 1] - 1); j++)
            {
              charInput[j - indices[i]] = NMEAChar[j + 1]; // Inserts the string value of speed in knots
            }

            knot = atof ( charInput );             // Derives double variable of speed in knots

            if (mph_kph)
            {
              speedvar = knot * 1.150779;             // Calculates mph as double variable
            }
            else
            {
              speedvar = knot * 1.852;              // Calculates km/h as double variable
            }
            // ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

            break;

          // ----------------------------------------------------------------------------------------
          case 8 :                                 // Retrieves date

            char dateChar[3];

            dateChar[0] = NMEAChar[indices[i] + 1];
            dateChar[1] = NMEAChar[indices[i] + 2];
            dy = atoi( dateChar );

            dateChar[0] = NMEAChar[indices[i] + 3];
            dateChar[1] = NMEAChar[indices[i] + 4];
            mnth = atoi( dateChar );

            dateChar[0] = NMEAChar[indices[i] + 5];
            dateChar[1] = NMEAChar[indices[i] + 6];
            yr = atoi( dateChar );

            //dateStr = String(yr) + "/" + String(mnth) + "/" + String(dy);

            break;
        }
      }

      setTime(hr, min, sec, dy, mnth, yr);

      if (DaylightSaving)
      {
        adjustTime(3600);
      }

      timeStr = String(hr) + ":" + String(min) + ":" + String(sec);
      dateStr = String(yr) + "/" + String(mnth) + "/" + String(dy);

      GPSTick = TRUE;
      //getTemperature();

      //Serial.println(dateStr + "," + timeStr + "," + knot + "," + instant_pulse_edgeRate + "," + pulse_edgeRate);
      Serial.println(dateStr + "," + timeStr + "," + knot + "," + String(metres_per_sec, DEC) + ", " + String(calibrationFactor, DEC));


      if (logOn == TRUE)
      {
        logData(dateStr + "," + timeStr + "," + knot + "," + String(metres_per_sec, DEC) + ", " + String(calibrationFactor, DEC));
      }

      digitalClockDisplay();

      // ----------------------------------------------------------------------------------------

      //speed_ = sec + 100;
      //      speed_ = sec;
      //      speed_100s = speed_ / 100;
      //      speed_10s = (speed_ - 100 * speed_100s) / 10;
      //      speed_1s = speed_ - 100 * speed_100s - 10 * speed_10s;
      //
      //      //oled.selectFont(Arial40Numbers);
      //      oled.selectFont(Arial_32x22_Numbers);
      //
      //      if (speed_100s == 0)
      //      {
      //        speed100sbox.clear();
      //        speed100sbox.println("");
      //      }
      //      else
      //      {
      //        speed10sbox.reset();
      //        speed100sbox.println(speed_100s);
      //      }
      //
      //      if (speed_10s == 0)
      //      {
      //        speed10sbox.clear();
      //        speed10sbox.println("");
      //      }
      //      else
      //      {
      //        speed10sbox.reset();
      //        speed10sbox.println(speed_10s);
      //
      //      }
      //      speed1sbox.reset();
      //      speed1sbox.println(speed_1s);
      //
      //      old_speed_100s = speed_100s;
      //      old_speed_10s = speed_10s;
      //
      //      oled.selectFont(Arial_Black_16);
      //      speedunits.reset();
      //      if (mph_kph)
      //      {
      //        speedunits.println("mph");
      //      }
      //      else
      //      {
      //        speedunits.println("kph");
      //      }
    }
  }
}

//--------------------------------------------------------------------------------------

void digitalClockDisplay()
// digital clock display of the time
{
  if (oldTime != 10 * hour() + minute())
  {
    oled.selectFont(Arial_15_mod);

    if (_24hrClock)
    {
      if (hour() < 10)
      {
        hourStr = "0" + String(hour());
      }
      hourStr =  String(hour());
    }
    else
    {
      if (hourFormat12() < 10)
      {
        hourStr = " " + String(hourFormat12());
      }
      hourStr =  String(hourFormat12());
    }

    if (minute() < 10)
    {
      minuteStr = "0" + String(minute());
    }
    else
    {
      minuteStr =  String(minute());
    }

    short_timeStr = hourStr + ":" + minuteStr;

    Serial.print(short_timeStr);

    timebox.println(short_timeStr);

    oldTime = 10 * hour() + minute();
  }

  if (oldDate != 10 * month() + day())
  {
    oled.selectFont(Arial_15_mod);
    daybox.println(day());
    oled.selectFont(Arial14);
    monthbox.println(monthShortStr(month()));
    oldDate = 10 * month() + day();
  }

  Serial.print("  ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(monthShortStr(month()));
  Serial.println();

}

// ----------------------------------------------------------------------------------------

void printDigits(int digits, String spacer)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  if (digits < 10)
  {
    Serial.print(spacer);
    timebox.print(spacer);
  }
  Serial.print(digits);
  timebox.print(digits);

}

// ***************************************************************************

