const byte irPin = 4,
           nbytes = 20;
byte       sendbytes = 0;
char      data[40];
uint32_t  ul;

const uint16_t T1L = 520, T15L = 9000,                          // LOW times
               T1P = 550, T3P  = 1660, T4P = 2200, T8P = 4440;  // PULSED times
               
byte cmdBytes[nbytes] = {0};

void sendCmd();
void _sendIt(uint16_t t1, uint16_t t2);

void setup() {
  Serial.begin(115200);
  pinMode(irPin, OUTPUT);
  digitalWrite(irPin, LOW);
}

void loop() {
  int i = 0;
  bool n = false;
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
    sendbytes = i-1;
    ul  =  0;
    for (int j = 0 ; j < strlen(data) ; j++) {
      ul *= 16;
      if (data[j] >= 'A' && data[j] <= 'F') {
        ul += (data[j] - 'A' + 10);
        cmdBytes[j] = (data[j] - 'A' + 10);
      }
      else if (data[j] >= 'a' && data[j] <= 'f') {
        ul += (data[j] - 'a' + 10);
        cmdBytes[j] = (data[j] - 'a' + 10);
      }
      else if(data[j] >= '0' && data[j] <= '9'){
        ul += data[j] - '0';
        cmdBytes[j] = (data[j] - '0');
      }
    }
    for(int k = 0 ; k < sendbytes; k++){
      Serial.println(cmdBytes[k]);
    }
    Serial.print("Received from Serial & Emitting: ");
    Serial.println(ul, HEX);
    delay(50);
    sendCmd();
  }
  
}

void sendCmd() {
  _sendIt(T15L, T8P);  // START stream 
  
  for (byte i = 0; i < sendbytes; i++) {
    for (byte j = 0; j < 4; j++) _sendIt(T1L, ((cmdBytes[i] << j) & 0x8  ? T3P : T1P)); 
  }
  _sendIt(T15L, T4P);  // END stream
  _sendIt(T15L, T4P);  //   2x  
}

void _sendIt(uint16_t t1, uint16_t t2) {  // 38KHz PULSED by OPEN-SMART Infrared Emitter
  digitalWrite(irPin, HIGH);
  delayMicroseconds(t1);
  digitalWrite(irPin, LOW);
  delayMicroseconds(t2);
}
