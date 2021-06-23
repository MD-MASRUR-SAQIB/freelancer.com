/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Team Abacus
   Date of creation: 01-Feb-21
   Project Name: Surokkha
   Last Modification: 02-Feb-21
   Modified by: Md. Masrur Saqib
*/

#ifndef __TIMESTAMP_H__
#define __TIMESTAMP_H__

#include<Arduino.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>

class timeStamp {
    String CTime, CDate;
    tmElements_t tm;
    bool config = false;
  public:
    timeStamp() {

    }
    String print2digits(int number) {
      if (number >= 0 && number < 10) {
        return "0" + String(number);
      }
      else
        return String(number);
    }
    String getTimeStamp() {
      if (RTC.read(tm)) {
        CTime = String(print2digits(tm.Hour) + ":"
                       + print2digits(tm.Minute))
                       ;
      }
      else {
        CTime = ("DS1307 error!");
      }
      return CTime;
    }

    void setRTC(String DT) {
      String TIME, DATE;

      int start = DT.indexOf(",");
      int end = DT.lastIndexOf("+");
      if ( start != -1 || end != -1) {
        TIME = DT.substring(start + 1, end);
        Serial.println(TIME);
      }

      const char *str = TIME.c_str();
      int Hour, Min, Sec;
      if (sscanf(str, "%d:%d:%d", &Hour, &Min, &Sec) != 3)
        return;
      tm.Hour = Hour;
      tm.Minute = Min;
      tm.Second = Sec;

      Serial.println(tm.Hour);
      Serial.println(tm.Minute);
      Serial.println(tm.Second);

      int Day, Year, Month;

      start = 0;
      end = DT.lastIndexOf(",");
      if ( start != -1 || end != -1) {
        DATE = DT.substring(start, end);
        Serial.println(DATE);
      }
      const char *str2 = DATE.c_str();
      if (sscanf(str2, "%d/%d/%d", &Year, &Month, &Day) != 3)
        return;
      tm.Day = Day;
      tm.Month = Month;
      tm.Year = CalendarYrToTm(Year + 2000);

      Serial.println(tm.Day);
      Serial.println(tm.Month);
      Serial.println(tm.Year);

      if (RTC.write(tm)) {
        config = true;
      }
    }

    String getFullDate() {
      if (RTC.read(tm)) {
        CDate = print2digits(tm.Day) + "-"
                + print2digits(tm.Month) + "-"
                + tmYearToCalendar(tm.Year);
      }
      else {
        CDate = ("DS1307 error!");
      }
      return CDate;
    }

    String getFullDateName() {
      if (RTC.read(tm)) {
        CDate = print2digits(tm.Day) + "_"
                + print2digits(tm.Month);
      }
      else {
        CDate = ("DS1307 error!");
      }
      return (String)CDate;
    }

    int getDateOnly() {
      int dateon;
      if (RTC.read(tm)) {
        dateon = (tm.Day);
      }
      else {
        dateon = -1;
      }
      return dateon;
    }

    int getHourOnly() {
      int dateon;
      if (RTC.read(tm)) {
        dateon = (tm.Hour);
      }
      else {
        dateon = -1;
      }
      return dateon;
    }

    int getMinutesOnly() {
      int dateon;
      if (RTC.read(tm)) {
        dateon = (tm.Minute);
      }
      else {
        dateon = -1;
      }
      return dateon;
    }
    
};

#endif
