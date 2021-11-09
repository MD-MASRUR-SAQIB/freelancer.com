#include <AbsMouse.h>

const int num_of_pb = 16; //Total push button connected to the device

int x_pos[16];
int y_pos[16];
int pinNo;

/*
   Start connecting from the 2 no. pin of Arduino
*/

void setup() {
  /*
    Input Mouse 1st click postion.
    The following code determines that the
    1st mouse click will be pressed at
    (x_pos[0],y_pos[0]) = (1,1) position.
    Rest of the mouse click positions will be like this.
  */
  x_pos[0] = 1135;
  y_pos[0] = 303;

  x_pos[1] = 1234; //Input Mouse 2nd click postion.
  y_pos[1] = 227; //Input Mouse 2nd click postion.

  x_pos[2] = 1371; //Input Mouse 3rd click postion.
  y_pos[2] = 321; //Input Mouse 3rd click postion.

  x_pos[3] = 1541; //Input Mouse 4th click postion.
  y_pos[3] = 350; //Input Mouse 4th click postion.

  x_pos[4] = 1618;
  y_pos[4] = 436;

  x_pos[5] = 730;
  y_pos[5] = 40;

  x_pos[6] = 873;
  y_pos[6] = 47;

  x_pos[7] = 985;
  y_pos[7] = 55;

  x_pos[8] = 1078;
  y_pos[8] = 55;

  x_pos[9] = 1222;
  y_pos[9] = 47;

  x_pos[10] = 1301;
  y_pos[10] = 68;

  x_pos[11] = 1416;
  y_pos[11] = 62;

  x_pos[12] = 1496;
  y_pos[12] = 62;

  x_pos[13] = 1583;
  y_pos[13] = 62;

  x_pos[14] = 1656;
  y_pos[14] = 63;

  x_pos[15] = 1708;
  y_pos[15] = 118;


  // Code For Push Button Starts
  Serial.begin(9600);
  for (int i = 0 ; i < num_of_pb ; i++) {
    pinNo = ((i + 2) <= 13) ? (i + 2) : (i + 6);
    pinMode(pinNo, INPUT_PULLUP);

  }
  delay(5000);
  AbsMouse.init(1920, 1080); //INPUT your screen resolution
  delay(3000);
  Serial.println("Starting Main Loop");
}

void loop() {
  for (int i = 2 ; i <= 13 ; i++) {
    if (digitalRead(i) == LOW) {
      AbsMouse.move(x_pos[i-2], y_pos[i-2]);
      delay(100);
      AbsMouse.press(MOUSE_LEFT);
      delay(100);
      AbsMouse.release(MOUSE_LEFT);
      AbsMouse.init(1920, 1080);
      delay(400);
    }
  }
  for (int i = 18 ; i <= 21   ; i++) {
    if (digitalRead(i) == LOW) {
      AbsMouse.move(x_pos[i-6], y_pos[i-6]);
      delay(100);
      AbsMouse.press(MOUSE_LEFT);
      delay(100);
      AbsMouse.release(MOUSE_LEFT);
      AbsMouse.init(1920, 1080);
      delay(400);
    }
  }
}
