/** to change as needed **
 *************************/
 
const byte irPin = 13;
const uint16_t maxCount = 300,  // max values sets (a set is 2)
               lowUs =    400,  // min. LOW in micros
               bdrUs =   1100,  // border between LOW and HIGH in micros
               higUS =   1800,  // max. HIGH in micros
               maxUs =  10000,  // max. long LOW or HIGH in micros
               maxMs =    100;  // timeout millis
/*************************/
bool pinIs = HIGH;
uint16_t count = 0,
                  lastCount = 0;
uint64_t us,
         ms,  
         val[2][maxCount + 1] = {0};

void getTimes();  // interrupt: time measurement 
void calcUs();

void setup() {
  Serial.begin(115200);
  while (!Serial); delay(1000);
  Serial.println("\n...\n");
  pinMode(irPin, INPUT);
  attachInterrupt(digitalPinToInterrupt(irPin), getTimes, CHANGE);
  Serial.println("\nWait for signal...\n");
  ms = millis();
}

void loop() {
  if (count == maxCount) calcUs();
  else if (count && (millis() > (ms + maxMs))) count = maxCount;
}

void getTimes() {
  us = micros();
  if (count == maxCount) return;
  if (digitalRead(irPin) != pinIs) {
    pinIs = !pinIs;
    val[pinIs][count] = us;
    if (pinIs) {
      lastCount = ++count;
      if (count == maxCount) count = maxCount;
    }
    ms = millis();
  }  
}

void calcUs() {  // calc values
  uint8_t _count = 0;
  uint64_t u[2];
  val[LOW][lastCount] = val[HIGH][lastCount - 1] + maxUs;
  for (uint16_t i = 0; i <= lastCount; i++) {
    u[LOW] = val[HIGH][i] - val[LOW][i];
    u[HIGH] = val[LOW][i + 1] - val[HIGH][i];
    if (u[LOW] > lowUs && u[LOW] < maxUs && u[HIGH] > lowUs && u[HIGH] < maxUs) {
      val[LOW][_count] = u[LOW];
      val[HIGH][_count++] = u[HIGH];
    }
  }
  if (_count > 8) {
    //Serial.print("number of LOW and PULSE couples: \n");
    //Serial.println(_count);
    //Serial.println(">>> 1ist:");
    for (uint16_t i = 0; i < _count; i++) {
      //Serial.print((int)val[LOW][i]);
      //Serial.print(" ");
      //Serial.println((int)val[HIGH][i]);
    }
    //Serial.print("\n>>> couples:\n  LOW PULSE bit_byte\n");
    byte c = 0,  // counter 0->3 bits
         v = 0,  // byte value from counter bits
         n = 0,  // 16 bits counter 
         bytes[_count / 4 + 1] = {0};
    char b[3] = "  ";
    boolean f = false;
    uint32_t gem[3] = {0};
    uint8_t  gemCount[3] = {0};
    for (uint8_t i = 0; i < _count; i++) {
      f = false;
      if (val[LOW][i] > bdrUs || val[HIGH][i] > higUS) {
        b[0] = ' ';
        b[1] = ' ';
      }
      else {
        if (val[LOW][i]  < bdrUs) {
          gem[0] += val[LOW][i];
          gemCount[0]++;
        }
        if (val[HIGH][i] < bdrUs) {
          gem[1] += val[HIGH][i]; 
          gemCount[1]++; 
          b[0] = '0';
        }
        else if (val[HIGH][i] < higUS) {
          gem[2] += val[HIGH][i]; 
          gemCount[2]++; 
          b[0] = '1';
        }
        if (c) v =  (v << 1);
        v += (b[0] == '1' ? 1 : 0);
        c++;
        if (c == 4) {
          b[1] = '_';
          c = 0;
          f = true;
        }
        else b[1] = ' ';
      }
      //Serial.print((int)val[LOW][i]);
      //Serial.print(" ");
      //Serial.print((int)val[HIGH][i]);
      //Serial.print(" ");
      //Serial.println(b);  // time set
      if (f) {
        bytes[n++] = v;
        //Serial.println(v,HEX);
        v = 0;
      }
      //else Serial.println();
    }
    Serial.println(">>> paired as 2x4 bits values:");
    for (byte i = 0; i < n; i += 2){
      Serial.print (bytes[i], HEX);
      Serial.print(bytes[i + 1], HEX);Serial.print(" ");
    }
    Serial.println("\n(normal)");
    for (byte i = 0; i < n; i += 2){
      Serial.print(~bytes[i] & 0x0F, HEX);
      Serial.print(~bytes[i + 1] & 0x0F, HEX);Serial.print(" ");
    }
    Serial.println();
    Serial.println(" (inverted, crosswise equal?) to test if a byte is send as inverted");
    if (gemCount[2]){
      Serial.print("Averages LOW: ");
      Serial.print(gem[0] / gemCount[0]);
      Serial.print(", Logic '0': ");
      Serial.print((gem[1] / gemCount[1]));
      Serial.print(", Logic '1': ");
      Serial.println(gem[2] / gemCount[2]);
    }
    else Serial.println("<<< ERROR >>> maxCount, lowUs, bdrUs, ...: declaration values problem!");
    Serial.println("--------------------------------------------------\n");
    for (uint16_t i = 0; i < maxCount; i++) val[LOW][i] = val[HIGH][i] = 0;  // clear array
  }
  count = 0;
}
