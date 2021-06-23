// USB host mouse from USB Host Shield Library. Install using Library Manager
#include <hidboot.h>
#include <usbhub.h>


// USB device mouse library included with Arduino IDE 1.8.5
#include <Mouse.h>

// Satisfy the IDE, which needs to see the include statment in the ino too.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

boolean isPressed = false;

class MouseRptParser : public MouseReportParser
{
  protected:
    void OnMouseMove  (MOUSEINFO *mi);
    void OnLeftButtonUp (MOUSEINFO *mi);
    void OnLeftButtonDown (MOUSEINFO *mi);
    void OnRightButtonUp  (MOUSEINFO *mi);
    void OnRightButtonDown  (MOUSEINFO *mi);
    void OnMiddleButtonUp (MOUSEINFO *mi);
    void OnMiddleButtonDown (MOUSEINFO *mi);
};
void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
  //Serial.print("dx=");
  //Serial.print(, DEC);
  //Serial.print(" dy=");
  //Serial.println(  
  Mouse.move(mi->dX, mi->dY, DEC);

};
void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{
  //Serial.println("L Butt Up");
  isPressed = false;
};
void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
  //Serial.println("L Butt Dn");
  isPressed = true;
};
void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
  //Serial.println("R Butt Up");
};
void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
  //Serial.println("R Butt Dn");
};
void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
  //Serial.println("M Butt Up");
};
void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
  //Serial.println("M Butt Dn");
};


USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);

MouseRptParser                               Prs;

void setup()
{
  //Serial.begin( 115200 );
#if !defined(__MIPSEL__)
  // while (!//Serial); // Wait for //Serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC //Serial connection //EDITADO PARA RODAR SEM O //Serial MONITOR
#endif
  //Serial.println("Start");

  if (Usb.Init() == -1)
    //Serial.println("OSC did not start.");

  delay( 200 );

  HidMouse.SetReportParser(0, &Prs);
  Mouse.begin();
}

void loop()
{


  Usb.Task();



}
