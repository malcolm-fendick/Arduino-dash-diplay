/*
  Speedo transducer used is the Hunter 65H from Sailes Marketing.
  It has dual output with 8 pulses per rev.
  165/65 R15 tyres on 4.5 rims have a circumference of 1.876 m,
  giving  4.264392324 pulses per meter.

  Red cable is +12v, Blue cable is GND, Signal 1 is Yellow, Signal 2 is Green

  The speedo transducer signal is connected to pin D22 via 5v-3.3v shifter
  and triggers on RISING edge.
*/

//***************************************************************************

void Speedo()
{
  // We need two pulses to calculate an interval between them

  if (pulse_edgeCount > 1)
  {
    instant_pulse_edgeRate = (float)(1000000 * (pulse_edgeCount - 1) / elapsedTime);
    pulse_edgeRate = instant_pulse_edgeRate;
  }
  else
  {
    pulse_edgeRate = 0;
  }

  // Now introduce some hysteresis (about 0.5 mph) to stop flickering of readout

  if (abs(pulse_edgeRate - old_pulse_edgeRate) < 0.2)
  {
    pulse_edgeRate = old_pulse_edgeRate;
  }

  else
  {
    old_pulse_edgeRate = pulse_edgeRate;
  }

  // ***************************

  metres_per_sec = pulse_edgeRate / calibrationFactor;

  // metres_per_sec = 5;

  km_per_hr = metres_per_sec * 3.6000;
  miles_per_hr = metres_per_sec * 2.2356;

  Serial.println(miles_per_hr);

  speed_ = miles_per_hr;

  if (speed_ != oldSpeed)

  {
    oled.selectFont(Droid_Sans_36_New);

    speedStr = String(speed_);

    unsigned int speed_width = oled.stringWidth(speedStr);
    unsigned int text_start = 70 - speed_width;

    oled.drawFilledBox(0, 54, 70, 80, BLACK);
    oled.drawString(text_start, 45, speedStr, WHITE, BLACK);


    //    if (speed_ < 10)
    //    {
    //      speedStr = "  " + String(speed_);
    //    }
    //
    //    else if (speed_ < 100)
    //    {
    //      speedStr = " " + String(speed_);
    //    }
    //    else
    //    {
    //      speedStr = String(speed_);
    //    }
    //
    //    speedbox.reset();
    //    speedbox.println(speedStr);

    oldSpeed = speed_;
  }

  speedoLoopTime = millis();
  GPSTick = FALSE;

  // Reset the pulse counter so we can start incrementing again
  pulse_edgeCount = 0;
  elapsedTime = 1000000;
  startCount = true;
  //  }
}

//***************************************************************************

void Speedo_pulse()
{
  /*
    This is invoked by interrupt 8 times per revolution of the speed sensor.
    The array records the times (micros) of the last 8 pulse edges so that
    the elapsedTime can be calculated over the last whole revolution.
  */

  holdTime = micros();

  // Increment the pulse counter
  if (pulse_edgeCount < 8)
  {
    pulse_edgeCount++;
  }

  // Reset the array at the start of the sampling period.
  if (startCount == true)
  {
    startCount = false;
    for (int x = 0; x < 8; x++)
    {
      intervalTime[x] = holdTime;
    }
  }

  // shift the last 7 intervalTimes up one register and assign the current micros()
  // to the first register.

  for (int x = 7; x > 0; x--)
  {
    intervalTime[x] = intervalTime[x - 1];
  }
  intervalTime[0] = holdTime;
  elapsedTime = intervalTime[0] - intervalTime[7];

  if (pulse_edgeCount > 9)
  {
    pulse_edgeCount = 9;
  }

}
//***************************************************************************













