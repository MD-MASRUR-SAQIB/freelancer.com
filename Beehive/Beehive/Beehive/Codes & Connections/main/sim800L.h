/*
   Author: Md. Masrur Saqib, CSE-18, RUET
   Organization: Team Abacus
   Date of creation: 01-Feb-21
   Project Name: Surokkha
   Last Modification: 02-Feb-21
   Modified by: Md. Masrur Saqib
*/

#ifndef __SIM800L_H__
#define __SIM800L_H__

#include<Arduino.h>

class sim800L {

    String operatorName, data, operatorSig, DT, incomingMessage;

  public:
    sim800L() {

    }

    begin() {
      Serial3.println("AT+CCLK?");
      Serial.println(Serial3.readString());
      Serial3.println("AT+CLTS=1");
      Serial.println(Serial3.readString());
      Serial3.println("AT+CLTS?");
      Serial.println(Serial3.readString());
      Serial3.println("AT&W");
      Serial.println(Serial3.readString());
      Serial3.println("AT+CMGF=1");
      Serial.println(Serial3.readString());
      Serial3.println("AT+CNMI=1,2,0,0,0");
      Serial.println(Serial3.readString());
      Serial3.println("AT+CMGD=1,4");
      Serial.println(Serial3.readString());
      delay(500);
    }

    String getOperator() {
      Serial3.println("AT+COPS=3,0");
      Serial3.readString();
      Serial3.println("AT+COPS?");
      delay(200);
      if (Serial3.available()) {
        data = Serial3.readString();
      }
      int start = data.indexOf("\"");
      int end = data.lastIndexOf("\"");
      if ( start != -1 || end != -1) {
        operatorName = data.substring(start + 1, end);
      }
      Serial.print("The operator Name is: ");
      Serial.println(operatorName);
      return operatorName;
    }

    String getStrength() {
      DT = "";
      Serial3.println("AT+CSQ?");
      Serial3.readString();
      Serial3.println("AT+CSQ");
      delay(200);
      if (Serial3.available()) {
        data = Serial3.readString();
      }
      int start = data.indexOf(": ");
      int end = data.lastIndexOf(",");
      if ( start != -1 || end != -1) {
        operatorSig = data.substring(start + 2, end);
      }
      int str = operatorSig.toInt();
      str = (str - 2) * 100 / 28;

      String condition = (String)str + " %";
      /*
        if(str < 10){
        condition = "Marginal";
        }
        else if(str < 15){
        condition = "OK";
        }
        else if(str < 20){
        condition = "Good";
        }
        else{
        condition = "Excellent";
        }
      */
      Serial.println(operatorSig);
      Serial.println(condition);
      return condition;
    }

    String getDT() {
      DT = "";
      Serial3.println("AT+CCLK?");
      delay(200);
      if (Serial3.available()) {
        data = Serial3.readString();
      }
      int start = data.indexOf("\"");
      int end = data.lastIndexOf("\"");
      if ( start != -1 || end != -1) {
        DT = data.substring(start + 1, end);
        Serial.println(DT);
      }

      return DT;
    }

    String getVali() {
      DT = "";
      delay(500);
      while(Serial3.available()) {
        data = Serial3.readString();
      }
      delay(500);
      Serial3.println("AT+CUSD=1,\"*123#\"");
      
      uint32_t l_timer = millis();
      while (millis() - l_timer < 5000) {
        while (Serial3.available()) {
          char l_char;
          l_char = Serial3.read();
          data += l_char;
          l_timer = millis();
          
        }
      }

      Serial.println(data);

      int start = data.indexOf("+CUSD: ") + 8;
      int end = data.length() - 1;
      if ( start != -1 || end != -1) {
        DT = data.substring(start + 1, end);
        Serial.println(DT);
      }

      return DT;
    }

    String incomingProcess() {
      DT = "";
      data = "";
      uint32_t l_timer = millis();
      while (millis() - l_timer < 250) {
        while (Serial3.available()) {
          char l_char;
          l_char = Serial3.read();
          data += l_char;
          l_timer = millis();
        }
      }

      incomingMessage = data;
      int start = data.indexOf("?");
      int end = data.lastIndexOf("?");
      if ( start != -1 || end != -1) {
        DT = data.substring(start + 1, end);
        Serial.println(DT);
      }

      return DT;
    }

    String saveMessage() {
      return incomingMessage;
    }

    void sendMessage(String Message, String textSendingNumber) {
      String serialData = "AT+CMGS=\"" + textSendingNumber + "\"";
      Serial3.println(serialData);
      delay(500);
      Serial3.print(Message);
      delay(500);
      Serial3.write(26);
      delay(500);
      Serial3.print("AT");
      delay(500);
      while (Serial3.available()) {
        data = Serial3.readString();
        Serial.println(data);
      }
      delay(500);
    }

};

#endif
