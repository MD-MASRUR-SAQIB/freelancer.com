#include <hidboot.h>
#include <usbhub.h>
#include <Mouse.h>
#include <SPI.h>

bool      isPressedLeft = false;
bool      isPressedLeftLong = false;
bool      isPressedRight = false;
bool      isPressedMiddle = false;
bool      isPressedSideUp = false;
bool      isPressedSideDown = false;

bool      recoilCorrection = false;

int       mouseclickLength = 100;//millisecond
uint64_t  recoilTime;
uint64_t  recoilSpeed = 3; //increase/decrease to change recoil speed
uint64_t  mouseClickTimeLeft;
uint64_t  mouseClickTimeRight;
uint64_t  mouseClickTimeMiddle;
uint64_t  mouseClickTimeSideUp;
uint64_t  mouseClickTimeSideDown;

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
    void OnSideButtonUpUp(MOUSEINFO *mi);
    void OnSideButtonUpDown(MOUSEINFO *mi);
    void OnSideButtonDownUp(MOUSEINFO *mi);
    void OnSideButtonDownDown(MOUSEINFO *mi);
};

void MouseRptParser::OnMouseMove(MOUSEINFO *mi)
{
  Mouse.move(mi->dX, mi->dY, mi->Wheel);
};

void MouseRptParser::OnLeftButtonUp (MOUSEINFO *mi)
{
  isPressedLeft = false;
};

void MouseRptParser::OnLeftButtonDown (MOUSEINFO *mi)
{
  isPressedLeft = true;
};

void MouseRptParser::OnRightButtonUp  (MOUSEINFO *mi)
{
  isPressedRight = false;
};

void MouseRptParser::OnRightButtonDown  (MOUSEINFO *mi)
{
  isPressedRight = true;
};

void MouseRptParser::OnMiddleButtonUp (MOUSEINFO *mi)
{
  isPressedMiddle = false;
};

void MouseRptParser::OnMiddleButtonDown (MOUSEINFO *mi)
{
  isPressedMiddle = true;
};

void MouseRptParser::OnSideButtonUpUp  (MOUSEINFO *mi)
{
  isPressedSideUp = false;
};

void MouseRptParser::OnSideButtonUpDown (MOUSEINFO *mi)
{
  isPressedSideUp = true;
};

void MouseRptParser::OnSideButtonDownUp  (MOUSEINFO *mi)
{
  isPressedSideDown = false;
};

void MouseRptParser::OnSideButtonDownDown (MOUSEINFO *mi)
{
  isPressedSideDown = true;
};

USB     Usb;
USBHub     Hub(&Usb);
HIDBoot<USB_HID_PROTOCOL_MOUSE>    HidMouse(&Usb);
MouseRptParser                     Prs;

void setup()
{
  Usb.Init();
  delay(200);
  HidMouse.SetReportParser(0, &Prs);
  Mouse.begin();
}

void loop()
{
  Usb.Task();

  if (isPressedLeft && !isPressedLeftLong) {
    Mouse.press(MOUSE_LEFT);
    mouseClickTimeLeft = millis();
    isPressedLeftLong = true;
  }
  else if (!isPressedLeft) {
    if (millis() - mouseClickTimeLeft > mouseclickLength) {
      Mouse.release(MOUSE_LEFT);
      isPressedLeftLong = false;
    }
  }

  while (millis() - mouseClickTimeLeft > mouseclickLength && isPressedLeft && isPressedLeftLong && recoilCorrection) {
    if (millis() - recoilTime > recoilSpeed) {
      Mouse.move(0, 1, 0);
      recoilTime = millis();
    }
    Usb.Task();
  }

  if (isPressedRight) {
    if (millis() - mouseClickTimeRight > 100) {
      Mouse.click(MOUSE_RIGHT);
      mouseClickTimeRight = millis();
    }
  }

  if (isPressedMiddle) {
    if (millis() - mouseClickTimeRight > 100) {
      Mouse.click(MOUSE_MIDDLE);
      mouseClickTimeMiddle = millis();
      if(recoilCorrection){
        recoilCorrection = false;
      }
      else{
        recoilCorrection = true;
      }
    }
  }

  if (isPressedSideUp) {
    if (millis() - mouseClickTimeSideUp > 100) {
      Mouse.click(MOUSE_SIDEUP);
      mouseClickTimeSideUp = millis();
    }
  }

  if (isPressedSideDown) {
    if (millis() - mouseClickTimeSideDown > 100) {
      Mouse.click(MOUSE_SIDEDOWN);
      mouseClickTimeSideDown = millis();
    }
  }

}
