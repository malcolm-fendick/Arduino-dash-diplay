/****************************************************************************
   K-G Logo
 ****************************************************************************
*/

#include <RightIndicator16x30x37.h>
#include <LeftIndicator16x30x37.h>
#include <K_G_Logo_100x43Neg.h>
#include <HighBeam16x37x37.h>
#include <ChargeWarning_16x22x14.h>
#include <OilWarning_16x22x8.h>

void K_G_Logo()
{
  oled.displayBMP(K_G_Logo_100x43Neg, 17, 55);
}

/****************************************************************************
   show warnings
 ****************************************************************************
*/
void showWarnings()
{
  leftIndOn = !digitalRead(leftIndicatorPin);
  rightIndOn = !digitalRead(rightIndicatorPin);
  highBeamIndOn = !digitalRead(highBeamIndicatorPin);
  oilWarningOn = digitalRead(oilWarningPin);
  chargeWarningOn = digitalRead(chargeWarningPin);

  // oilWarningOn = !TRUE;
  // chargeWarningOn = !TRUE;
  // highBeamIndOn = !TRUE;

  //--------------------------------------------------------------------------

  if (oilWarningOn == TRUE  || chargeWarningOn == TRUE)
  {
    if (oilWarningOn == TRUE)
    {
      oled.displayBMP(OilWarning_16x22x8, 45, 110);
    }
    else
    {
      oled.drawFilledBox(45, 110, 81, 127, BLACK);
    }
    if (chargeWarningOn == TRUE)
    {
      oled.displayBMP(ChargeWarning_16x22x14, 45, 91);
    }
    else
    {
      oled.drawFilledBox(45, 91, 81, 104, BLACK);
    }
  }
  else if (oilWarningOn == FALSE && chargeWarningOn == FALSE)
  {
    if (highBeamIndOn == TRUE)
    {
      oled.displayBMP(HighBeam16x37x37, 45, 91);
    }
    else
    {
      //oled.drawFilledBox(45, 91, 81, 127, YELLOW);
      oled.drawFilledBox(45, 91, 81, 127, BLACK);
    }
  }

  //--------------------------------------------------------------------------

  if (leftIndOn == TRUE)
  {
    oled.displayBMP(LeftIndicator16x30x37, 0, 91);
  }
  else
  {
    //oled.drawFilledBox(0, 91, 29, 127, BLUE);
    oled.drawFilledBox(0, 91, 29, 127, BLACK);
  }

  //--------------------------------------------------------------------------

  if (rightIndOn == TRUE)
  {
    oled.displayBMP(RightIndicator16x30x37, 98, 91);
  }
  else
  {
    //oled.drawFilledBox(98, 91, 127, 127, RED);
    oled.drawFilledBox(98, 91, 127, 127, BLACK);


  }
}
/****************************************************************************

 ****************************************************************************
*/
