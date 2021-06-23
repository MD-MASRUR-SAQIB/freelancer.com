#include <usbhid.h>
#include <hiduniversal.h>
#include <usbhub.h>

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif
#include <SPI.h>

#include "hidmouserptparser.h"

USB Usb;
USBHub Hub(&Usb);
HIDUniversal Hid(&Usb);
HIDMouseEvents MouEvents;
HIDMouseReportParser Mou(&MouEvents);

void setup() {
        Serial.begin(9600);
#if !defined(__MIPSEL__)
        while (!Serial); // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
#endif
        Serial.println("Start");

        if (Usb.Init() == -1)
                Serial.println("OSC did not start.");

        delay(200);

        if (!Hid.SetReportParser(0, &Mou))
                ErrorMessage<uint8_t > (PSTR("SetReportParser"), 1);
}

void loop() {
        Usb.Task();
}
