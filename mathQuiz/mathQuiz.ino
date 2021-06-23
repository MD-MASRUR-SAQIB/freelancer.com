#include <LiquidCrystal.h>
#include <Keypad.h>

#define   pressedSelect       digitalRead(A0)
#define   pressedIncrement    digitalRead(A1)
#define   pressedDecrement    digitalRead(A2)
#define   pressedDelete       digitalRead(A3)

const byte ROWS = 4;
const byte COLS = 4;
uint8_t    Difficulty = 1;
uint8_t    points = 0;

char keys[ROWS][COLS] = {
  {'1','2','3', 'A'},
  {'4','5','6', 'B'},
  {'7','8','9', 'C'},
  {'*','0','#', 'D'}
};

byte rowPins[ROWS] = {0, A4, 2, 3};
byte colPins[COLS] = {4, 5, 6, 7};

int currentLed     = 14;

LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(A5));


  
  pinMode(14, OUTPUT);
  pinMode(15, OUTPUT);
  pinMode(16, OUTPUT);
  pinMode(17, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
  
  lcd.begin(16, 2);
  Serial.println("Display Initiated:");
  lcd.setCursor(0, 0);
  lcd.print(" Welcome to the ");
  lcd.setCursor(0, 1);
  lcd.print("    Math Quiz   ");
  delay(2000);
}

void loop() {
  digitalWrite(19, LOW);
  points = 0;
  selectDifficulty();

  while (!pressedSelect) {
    if (pressedDelete) {
      Serial.println("Pressed Delete!");
      selectDifficulty();
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Starting with");
  lcd.setCursor(0, 1);
  lcd.print("difficulty: ");
  lcd.print(Difficulty);
  delay(1000);
  for (int i = 0 ; i < 10 ; i++) {
    randomSeed(analogRead(A5));
    if (Difficulty == 1) {
      randomMathGenerator(1, 9);
    }
    else if (Difficulty == 2) {
      randomMathGenerator(10, 20);
    }
    else if (Difficulty == 3) {
      randomMathGenerator(21, 100);
    }
    else if (Difficulty == 4) {
      randomMathGenerator(101, 1000);
    }
    else {
      randomMathGenerator(1001, 10000);
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("You got ");
  lcd.print(points);
  lcd.print(" / 10");
  if(points > 9){
    lcd.setCursor(0, 1);
    lcd.print("Congratulations!");
  }
  if(points >= 8){
    digitalWrite(19, HIGH);
  }
  else{
    digitalWrite(19, LOW);
  }

  delay(10000);
}

void randomMathGenerator(int High, int Low) {
  randomSeed(analogRead(A5));
  lcd.clear();
  int sign = random(0, 3);
  if (sign == 0) {
    int random1 = random(Low, High);
    int random2 = random(Low, High);
    lcd.setCursor(0, 0);
    lcd.print(random1);
    lcd.print(" + ");
    lcd.print(random2);
    lcd.print(" =?");
    if (getValue() == random1 + random2) {
      points++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Correct Answer!!");
      delay(800);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Wrong Answer!!");
      delay(800);
    }
  }

  else if (sign == 1) {
    int random1 = random(Low, High);
    int random2 = random(Low, High);
    lcd.setCursor(0, 0);
    lcd.print((random1 > random2) ? random1 : random2);
    lcd.print(" - ");
    lcd.print((random1 < random2) ? random1 : random2);
    lcd.print(" =?");

    if (getValue() == abs(random1 - random2)) {
      points++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Correct Answer!!");
      delay(800);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Wrong Answer!!");
      delay(800);
    }
  }

  else if (sign == 2) {
    int random1 = random(Low, High);
    int random2 = random(Low, High);
    lcd.setCursor(0, 0);
    lcd.print(random1);
    lcd.print(" X ");
    lcd.print(random2);
    lcd.print(" =?");

    if (getValue() == random1 * random2) {
      points++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Correct Answer!!");
      delay(800);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Wrong Answer!!");
      delay(800);
    }
  }

  else if (sign == 3) {
    int random1 = random(Low, High);
    int random2 = random(1, random1);

    int result = random1 / random2;

    random1 = result * random2;


    lcd.setCursor(0, 0);
    lcd.print(random1);
    lcd.print(" / ");
    lcd.print(random2);
    lcd.print(" =?");

    if (getValue() == random1 / random2) {
      points++;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Correct Answer!!");
      delay(800);
    }
    else {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.println("Wrong Answer!!");
      delay(800);
    }
  }

}

int getValue() {
  int num = 0;
  lcd.setCursor(0, 1);
  lcd.print(num);
  while (!pressedSelect) {
    char key = keypad.getKey();
    if (key) {
      num *= 10;
      num += (key - 48);
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(num);
    }
    if (pressedDelete) {
      num /= 10;
      lcd.setCursor(0, 1);
      lcd.print("                ");
      lcd.setCursor(0, 1);
      lcd.print(num);
      delay(500);
    }
  }
  return num;
}


void selectDifficulty() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Please Select   ");
  lcd.setCursor(0, 1);
  lcd.print("difficulty: ");
  lcd.setCursor(12, 1);
  lcd.print(Difficulty);
  turnOff();
  digitalWrite(currentLed, HIGH);
  while (!pressedSelect) {
    if (pressedIncrement) {
      currentLed++;
      Difficulty++;
      if (Difficulty > 5) {
        Difficulty = 5;
        currentLed = 18;
      }
      lcd.setCursor(12, 1);
      lcd.print(Difficulty);
      delay(500);
    }
    if (pressedDecrement) {
      Difficulty--;
      currentLed--;
      if (Difficulty < 1) {
        Difficulty = 1;
        currentLed = 14;
      }
      lcd.setCursor(12, 1);
      lcd.print(Difficulty);
      delay(500);
    }
    turnOff();
    digitalWrite(currentLed, HIGH);
  }
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Are you sure to");
  lcd.setCursor(0, 1);
  lcd.print("select ");
  lcd.setCursor(7, 1);
  lcd.print(Difficulty);
  lcd.print("?");
  delay(500);
}

void turnOff(){
  for(int i = 14; i < 19 ; i++){
    digitalWrite(i, LOW);
  }
}

unsigned long generateRealRandom(int min, int max){
  randomSeed(analogRead(A5));
  uint64_t realRandom = random(1, 10) + random(1, 100) + random(1, 1000) + random(1, 10000);
  realRandom %= (max - min);
  realRandom += min;
  return realRandom;
}
