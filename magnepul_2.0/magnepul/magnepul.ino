uint8_t pinTDCInrpt       = 2;
uint8_t pinCrankInrpt     = 3;
uint8_t pinThrottle       = A0;

uint8_t cyl1Offset        = 0;
uint8_t cyl2Offset        = 127; //180 degrees
uint8_t crankAngle        = 0;
uint8_t crankSubPulses    = 0;
uint8_t crankPulses       = 0;
uint8_t crankVelocity     = 0;
uint8_t throttlePosition  = 0;
uint8_t cyl1StartAngle    = 0;
uint8_t cyl1Duration      = 0;
uint8_t cyl2StartAngle    = 0;
uint8_t cyl2Duration      = 0;
uint8_t cyl1Angle         = 0;
uint8_t cyl2Angle         = 0;
uint8_t cylXStartAngle    = 0;
uint8_t cylXDuration      = 0;
uint8_t flag20HzInt       = 0;
uint16_t ADCvalue         = 0;

bool cyl1Active           = false;
bool cyl2Active           = false;

const uint16_t timer1Load = 0;
const uint16_t timer1Comp = 625;




const uint8_t lookUpCylXStartPos[16][16] =
{
  {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
  {0,  8,  7,  5,  5,  4,  4,  2,  2,  2,  1,  1,  1,  1,  1,  1},
  {0,  9,  8,  7,  7,  5,  5,  4,  4,  4,  2,  2,  2,  2,  2,  2},
  {0, 11,  9,  8,  8,  7,  7,  5,  5,  5,  4,  4,  4,  4,  4,  4},
  {0, 12, 11,  9,  9,  8,  8,  7,  7,  7,  5,  5,  5,  5,  5,  5},
  {0, 14, 12, 11, 11,  9,  9,  8,  8,  8,  7,  7,  7,  7,  7,  7},
  {0, 15, 14, 12, 12, 11, 11,  9,  9,  9,  8,  8,  8,  8,  8,  8},
  {0, 16, 15, 14, 14, 12, 12, 11, 11, 11,  9,  9,  9,  9,  9,  9},
  {0, 18, 16, 15, 15, 14, 14, 12, 12, 12, 11, 11, 11, 11, 11, 11},
  {0, 19, 18, 16, 16, 15, 15, 14, 14, 14, 12, 12, 12, 12, 12, 12},
  {0, 21, 19, 18, 18, 16, 16, 15, 15, 15, 14, 14, 14, 14, 14, 14},
  {0, 22, 21, 19, 19, 18, 18, 16, 16, 16, 15, 15, 15, 15, 15, 15},
  {0, 23, 22, 21, 21, 19, 19, 18, 18, 18, 16, 16, 16, 16, 16, 16},
  {0, 25, 23, 22, 22, 21, 21, 19, 19, 19, 18, 18, 18, 18, 18, 18},
  {0, 26, 25, 23, 23, 22, 22, 21, 21, 21, 19, 19, 19, 19, 19, 19},
  {0, 28, 26, 25, 25, 23, 23, 22, 22, 22, 21, 21, 21, 21, 21, 21}
};

const uint8_t lookUpCylXDuration[16][16] =
{
   {0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0},
   {0,  4,  8, 12, 16, 21, 25, 29, 33, 37, 42, 46, 50, 54, 59, 63},
   {0,  4,  8, 12, 17, 21, 25, 30, 34, 38, 43, 47, 51, 56, 60, 64},
   {0,  4,  8, 13, 17, 22, 26, 30, 35, 39, 44, 48, 52, 57, 61, 66},
   {0,  4,  9, 13, 18, 22, 27, 31, 36, 40, 45, 49, 54, 58, 63, 67},
   {0,  4,  9, 13, 18, 22, 27, 32, 36, 41, 45, 50, 55, 59, 64, 68},
   {0,  4,  9, 14, 18, 23, 28, 32, 37, 42, 46, 51, 56, 60, 65, 70},
   {0,  4,  9, 14, 19, 23, 28, 33, 38, 43, 47, 52, 57, 62, 66, 71},
   {0,  4,  9, 14, 19, 24, 29, 34, 39, 43, 48, 53, 58, 63, 68, 73},
   {0,  4,  9, 14, 19, 24, 29, 34, 39, 44, 49, 54, 59, 64, 69, 74},
   {0,  5, 10, 15, 20, 25, 30, 35, 40, 45, 50, 55, 60, 65, 70, 75},
   {0,  5, 10, 15, 20, 25, 30, 36, 41, 46, 51, 56, 61, 67, 72, 77},
   {0,  5, 10, 15, 21, 26, 31, 36, 42, 47, 52, 57, 63, 68, 73, 78},
   {0,  5, 10, 16, 21, 26, 32, 37, 42, 48, 53, 58, 64, 69, 74, 80},
   {0,  5, 10, 16, 21, 27, 32, 38, 43, 48, 54, 59, 65, 70, 76, 81},
   {0,  5, 11, 16, 22, 28, 33, 39, 45, 50, 56, 61, 67, 73, 78, 84}
};

void setup() {
  attachInterrupt(digitalPinToInterrupt(pinTDCInrpt), TDCInterrupt, FALLING);
  attachInterrupt(digitalPinToInterrupt(pinCrankInrpt), crankAngleInterrupt, CHANGE);

  TCCR1A  = 0;
  TCCR1B |=  (1 << CS12);
  TCCR1B &= ~(1 << CS11);
  TCCR1B &= ~(1 << CS10);
  TCNT1 = timer1Load;
  OCR1A = timer1Comp;
  TIMSK1 = (1 << OCIE1A);
  sei();
}

void loop() {

}

ISR(TIMER1_COMPA_vect){ // 100hzInterrupt
  TCNT1 = timer1Load;
  
  ADCvalue = analogRead(pinThrottle);
  throttlePosition = ADCvalue >> 2;

  cylXStartAngle = lookUpCylXStartPos[crankVelocity >> 4][throttlePosition >> 4];
  cylXDuration   = lookUpCylXDuration[crankVelocity >> 4][throttlePosition >> 4];

  cyl1StartAngle = cylXStartAngle + cyl1Offset;
  cyl2StartAngle = cylXStartAngle + cyl2Offset;
  
  cyl1Duration = cylXDuration;
  cyl2Duration = cylXDuration;

  flag20HzInt++;

  if(flag20HzInt >= 5){
    interrupt20Hz();
    flag20HzInt = 0;
  }
}

void interrupt20Hz(){
  crankVelocity = crankPulses;
  crankSubPulses = 255;
  crankPulses = 255;
}

void crankAngleInterrupt(){
  crankAngle++;
  crankSubPulses++;
  if(crankSubPulses % 8){
    cyl1Angle = crankAngle - cyl1StartAngle;
    if(cyl1Angle > cyl1Duration){
      cyl1Active = false;
    }
    else{
      cyl1Active = true;
    }

    cyl2Angle = crankAngle - cyl2StartAngle;
    if(cyl2Angle > cyl2Duration){
      cyl2Active = false;
    }
    else{
      cyl2Active = true;
    }
  }
  else{
    crankPulses++;
  }
}

void TDCInterrupt(){
  crankAngle = 255;
}
