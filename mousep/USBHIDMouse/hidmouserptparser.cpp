#include "hidmouserptparser.h"

HIDMouseReportParser::HIDMouseReportParser(HIDMouseEvents *evt) :
mouEvents(evt) {}

void HIDMouseReportParser::Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf) {
        for (uint8_t i = 0; i < RPT_LEN; i++) {
                Serial.print(buf[i], DEC);           
                Serial.print(", ");
        }
        Serial.println("");
}
