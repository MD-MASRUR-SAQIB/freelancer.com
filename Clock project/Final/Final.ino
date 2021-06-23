// C++ code
//
#define B1		9
#define B2		10
#define B3		11
#define B4		12
#define BUZZER	8
#define TEMP36	A0
#define BUTTON_DELAY	200		// 200 ms
#define SNOOZE_TIME		300000 	// 5 min
#define HOLD_TIME		1	 	// 3 sec


#include<LiquidCrystal.h>

LiquidCrystal lcd(7,6,5,4,3,2);

volatile unsigned long second_int = 0;
int button_counter = 0;
int second = 0;
int minute = 0;
int hour = 0;
int actual_hour = 0;
int alarm_minute = 0;
int alarm_hour = 0;
int alarm_hour_ampm = 0;
bool alarm_on_off = false; 	// initially off
bool alarm_am_pm = false; 	// false = am; true pm
bool am_pm = false; 		// false = am; true pm
bool use_am_pm = true;
bool alarm_Snooze_st = false;
bool temp_c_f_status = false; // false -> degree celsius
unsigned long alarm_Snooze_time = 0;
unsigned long button_long_press_time = 0;

void time();
void alarmTime();
float temp(bool _c_f);
void display(bool _type);
void checkButton();
void checkAlarm();
void setTime();
void setAlarm();
void blink(int _index);

byte alarm[8]={B00100,B01010,B01010,B01010,B10001,B01110,B00100,B00000};
byte cross[8]={B00000,B10001,B01010,B00100,B01010,B10001,B00000,B00000};

void setup()
{
  TCCR1B = TCCR1A = TCCR1C = 0x00; // reset all TIMER1/COUNTER1 register
  TCCR1B = 1 << CS12;	// clkI/O/256 (From prescaler)
  TIMSK1 = 1 << 0;		// Overflow TIMER1 Interrupt Enable
  TCNT1 = 3036;			// (preload)65536-62500 = 3036; 62500 -> 1 sec
  pinMode(B1, INPUT_PULLUP);
  pinMode(B2, INPUT_PULLUP);
  pinMode(B3, INPUT_PULLUP);
  pinMode(B4, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  
  lcd.createChar(0,alarm);
  lcd.createChar(1,cross);
  lcd.begin(16,2);
  lcd.print("  Alarm Clock!  "); // 15 char
  delay(1500);
}

void loop()
{
  delay(10); // Wait for 10 millisecond(s)
  time();
  display(false);
  checkButton();
  checkAlarm();
}


void checkAlarm(){
  if(alarm_on_off){
    if(alarm_hour == actual_hour){
      if(millis()-alarm_Snooze_time < SNOOZE_TIME  && alarm_Snooze_st){ // 5 min
        digitalWrite(BUZZER, LOW); 	// 	Deactivated
      }
      else if(minute >= alarm_minute){
        digitalWrite(BUZZER, HIGH); // 	Activated
      }
      else{
        digitalWrite(BUZZER, LOW); 	// 	Deactivated
      }
    }
  }
}

void checkButton(){
  if(!digitalRead(B1)){
    use_am_pm = !use_am_pm;
    button_long_press_time = second_int;
    delay(BUTTON_DELAY);
    while(!digitalRead(B1)){
      if(second_int - button_long_press_time >= HOLD_TIME){ // 3 sec
        use_am_pm = !use_am_pm;
        setTime();
        break;
      }
    }
  }
  else if(!digitalRead(B2)){
    alarm_on_off = !alarm_on_off;
    button_long_press_time = second_int;
    delay(BUTTON_DELAY);
    while(!digitalRead(B2)){
      if(second_int - button_long_press_time >= HOLD_TIME){ // 3 sec
        setAlarm();
        alarm_on_off = true;
        break;
      }
    }
  }
  else if(!digitalRead(B3)){
    temp_c_f_status = !temp_c_f_status;
    delay(BUTTON_DELAY);
  }
  else if(!digitalRead(B4)){
    alarm_Snooze_time = millis();
    alarm_Snooze_st = !alarm_Snooze_st;
    delay(BUTTON_DELAY);
  }
}

void setTime(){
  //Serial.println("Set min");
  lcd.setCursor(0,0);			//thisCol, thisRow
  lcd.print("    Set Time    "); 		// max 16
  delay(1500);
  button_counter = 1;
  display(false);
  int sec_count = 0;
  while(1){
    if(!digitalRead(B1)){
      button_counter ++;
      delay(BUTTON_DELAY);
      if(button_counter >= 3){
        break;
      }
    }
    else if(!digitalRead(B3)){
      if(button_counter == 1){
        second_int += 60;
        sec_count ++;
        if(sec_count == 60){
          sec_count = 0;
          second_int -= 3600;
        }
      }
      else if(button_counter == 2){
        second_int += 3600;
      }
      
      delay(BUTTON_DELAY);
    }
    
    time();
    display(false);
  }
}

void setAlarm(){
  alarmTime();
  lcd.setCursor(0,1);			//thisCol, thisRow
  lcd.print("   Set Alarm    "); 		// max 16
  delay(1500);
  button_counter = 1;
  display(true);
  while(1){
    if(!digitalRead(B2)){
      button_counter ++;
      delay(BUTTON_DELAY);
      if(button_counter >= 3){
        break;
      }
    }
    else if(!digitalRead(B3)){
      if(button_counter == 1){
        alarm_minute ++;
      }
      else if(button_counter == 2){
        alarm_hour ++;
      }
      delay(BUTTON_DELAY);
    }
    
    alarmTime();
    display(true);
  }
}

void alarmTime(){
  alarm_minute = alarm_minute % 60;
  alarm_hour = alarm_hour % 24;
  alarm_hour_ampm = alarm_hour;
  
  if(use_am_pm){
    if(alarm_hour_ampm >= 12){				// check AM/PM
      alarm_am_pm = true;				// pm
    }
    else{
      alarm_am_pm = false; 		    // am
    }

    if(alarm_hour_ampm > 12){				// 13 to 23, make it 1 to 11
      alarm_hour_ampm -= 12;
    }else if(alarm_hour_ampm == 0){	    // 24(0), make it 12
      alarm_hour_ampm = 12;
    }
  }
}


void display(bool _type){
  if(_type){					// alarm
    lcd.setCursor(0,1);			//thisCol, thisRow
    lcd.print("Alarm "); 		// max 6
    
	blink(2);
    lcd.print(alarm_hour_ampm); // max 2
    lcd.print(':');				// 1
    
	blink(1);
    lcd.print(alarm_minute);	// max 2
    if(use_am_pm){
      lcd.print(' ');			// 1
      lcd.print(alarm_am_pm ? "PM" : "AM"); // max 2
    }
    
    lcd.print("     ");	// 5
  }
  else{						// time
    lcd.setCursor(0,0);		//thisCol, thisRow
    lcd.print("Time "); 	// max 5
    
	blink(2);
    lcd.print(hour); 		// max 2
    lcd.print(':'); 		// 1
    
	blink(1);
    lcd.print(minute); 	// max 2
    lcd.print(':');		// 1
    lcd.print(second);	// max 2
    if(use_am_pm){
      lcd.print(' ');	// 1
      lcd.print(am_pm ? "PM" : "AM");	// max 2
    }
    
    lcd.print("     ");		// 5
    lcd.setCursor(0,1);		//thisCol, thisRow
    lcd.print("Temp ");		// 5
    lcd.print(temp(temp_c_f_status)); 		// max 6, min 4
    lcd.print((char)178); 	// 1
    lcd.print(temp_c_f_status ? 'F' : 'C'); // 1
    lcd.print(' ');			// 1
    
    lcd.setCursor(14,1);	// thisCol, thisRow
    lcd.write(byte(0)); 	// alarm
    if(!alarm_on_off || alarm_Snooze_st){
      lcd.write(byte(1)); 	// cross
    }
    lcd.print("  ");		// 2
  }
}

void blink(int _index){
  if(button_counter == _index){
    lcd.blink();
    delay(50);
    lcd.noBlink();
  }
}

void time(){
  second = second_int % 60;
  minute = second_int/60;
  hour = minute/60;
  minute = minute % 60;
  hour = hour % 24;
  actual_hour = hour;
  if(use_am_pm){
    if(hour >= 12){				// check AM/PM
      am_pm = true;				// pm
    }
    else{
      am_pm = false; 		    // am
    }

    if(hour > 12){				// 13 to 23, make it 1 to 11
      hour -= 12;
    }else if(hour == 0){	    // 24(0), make it 12
      hour = 12;
    }
  }
}

float temp(bool _c_f){
  float _temp = analogRead(TEMP36);   //read the analog sensor and store it
  _temp = _temp / 1024;   			  //find percentage of input reading
  _temp = _temp * 5.0;                //multiply by 5V to get voltage
  _temp = _temp - 0.5;                //Subtract the offset 
  _temp = _temp * 100;                //Convert to degrees 
  if(_c_f){
    _temp = (_temp * (9.0/5.0)) + 32.0;
  }
  return _temp;
}

ISR(TIMER1_OVF_vect){
  TCNT1 = 3036;			// (preload)65536-62500 = 3036; 62500 -> 1 sec
  second_int ++;
}