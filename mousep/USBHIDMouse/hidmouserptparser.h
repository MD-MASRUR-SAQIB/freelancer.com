#if !defined(__HIDMOUSERPTPARSER_H__)
#define __HIDMOUSERPTPARSER_H__

#include <usbhid.h>

class HIDMouseEvents {};

#define RPT_LEN  8

class HIDMouseReportParser : public HIDReportParser {
        HIDMouseEvents *mouEvents;

public:
        HIDMouseReportParser(HIDMouseEvents *evt);

        virtual void Parse(USBHID *hid, bool is_rpt_id, uint8_t len, uint8_t *buf);
};

#endif // __HIDMOUSERPTPARSER_H__
