void logData(String myData)
{
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.

  dataFile = SD.open("datalog.csv", FILE_WRITE);

  // if the file is available, write to it:
  if (dataFile)
  {
    dataFile.println(myData);
    dataFile.close();
    // print to the serial port too:
    Serial.println("Logging " + myData);
  }
  // if the file isn't open, pop up an error:
  else
  {
    Serial.println("error opening datalog.csv");
  }
}
