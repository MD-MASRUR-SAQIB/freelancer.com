void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println(generateRealRandom(1, 1000));
  delay(100);
}

unsigned long generateRealRandom(int min, int max){
  randomSeed(analogRead(A5));
  uint64_t realRandom = random(1, 10) + random(1, 100) + random(1, 1000) + random(1, 10000);
  realRandom %= (max - min);
  realRandom += min;
  return realRandom;
}
