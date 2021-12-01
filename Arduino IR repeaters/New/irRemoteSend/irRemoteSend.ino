#include <IRremote.hpp>

#define IR_SEND_PIN 3

int       i;
bool      n;
char      data[20];
uint32_t  ul;
uint16_t  sAddress;
uint8_t   sCommand;
uint8_t   sRepeats;

void setup() {
  Serial.begin(115200);
  IrSender.begin(IR_SEND_PIN, false);
  Serial.println("Starting IR sender.....");
}

void loop() {
  i = 0;
  
  while (Serial.available()) {
    data[i] = Serial.read();
    Serial.print(data[i]);
    i++;
    n = true;
    delay(20);
  }
  
  if (n) {
    n = false;
    data[i - 1] = '\0';
    ul  =  0;
    for (int j = 0 ; j < strlen(data) ; j++) {
      ul *= 16;
      if (data[j] >= 'A' && data[j] <= 'F') {
        ul += (data[j] - 'A' + 10);
      }
      else if (data[j] >= 'a' && data[j] <= 'f') {
        ul += (data[j] - 'a' + 10);
      }
      else if(data[j] >= '0' && data[j] <= '9'){
        ul += data[j] - '0';
      }
    }
    Serial.print("Received from Serial & Emitting: ");
    Serial.println(ul, HEX);
    delay(50);
    
    IrSender.sendNECRaw(ul, 0);
  }
}
