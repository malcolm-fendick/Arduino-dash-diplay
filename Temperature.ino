/*
  Signal (yellow) wire from sensor connected to pin 24 which is pulled high by
  4k7 resistor.
*/


void getTemperature()
{
  sensors.requestTemperatures(); // Send the command to get temperatures

  float tempC = sensors.getTempC(insideThermometer);

  tempC_diff = tempC - old_tempC;
  tempC_diff = abs(tempC_diff);

  if (tempC_diff < .45)
  {
    tempC = old_tempC;
  }
  old_tempC = tempC;

  if (tempC <= 3)
  {
    tempAlert = TRUE;
  }
  else
  {
    tempAlert = FALSE;
  }

  if (tempC >= 0)
  {
    tempPos = TRUE;
  }
  else
  {
    tempPos = FALSE;
    tempC = -tempC;
  }

  int TempReadingInt = int(tempC);
  int TempReadingFrac = int(10 * tempC) - 10 * int(tempC);

  String TempReadingString =  String(TempReadingInt) + "." + String(TempReadingFrac);

  if (tempPos == TRUE)
  {
    TempReadingString = "+" + TempReadingString + ";" + "C"; // "," = "+", ";" = "°C"
  }
  else
  {
    TempReadingString = "-" + TempReadingString + ";" + "C"; // ";" = "°C"
  }

  Serial.print("Temperature = ");
  Serial.println(TempReadingString);

  oled.selectFont(Arial_15_mod);
  temp.reset();
  if (tempAlert == TRUE)
  {
    temp.setForegroundColour(BLUE);
  }
  else
  {
    temp.setForegroundColour(GREY);
  }

  temp.println(TempReadingString);
}
