#include "HX711.h"
//#include <AbsMouse.h>
#include <Mouse.h>
//#include <MouseTo.h>

const int num_of_scale = 1; //Total Load cell connected to the device
int sensitivity = 100000; // This is pushing sensitivity.

long x_pos[num_of_scale];
long y_pos[num_of_scale];

HX711 scale[num_of_scale];
long init_val[num_of_scale];

/*
   You have to connect 2 pins of each HX711 with Arduino.
   First pin is marked as DT/Dout
   Second pin is marked as SCK
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
  x_pos[0] = 100;
  y_pos[0] = 100;

  x_pos[1] = 1; //Input Mouse 2nd click postion.
  y_pos[1] = 1; //Input Mouse 2nd click postion.

  x_pos[2] = 1; //Input Mouse 3rd click postion.
  y_pos[2] = 1; //Input Mouse 3rd click postion.

  x_pos[3] = 1; //Input Mouse 4th click postion.
  y_pos[3] = 1; //Input Mouse 4th click postion.

  x_pos[4] = 1;
  y_pos[4] = 1;

  x_pos[5] = 1;
  y_pos[5] = 1;

  x_pos[6] = 1;
  y_pos[6] = 1;

  x_pos[7] = 1;
  y_pos[7] = 1;

  x_pos[8] = 1;
  y_pos[8] = 1;

  x_pos[9] = 1;
  y_pos[9] = 1;

  x_pos[10] = 1;
  y_pos[10] = 1;

  x_pos[11] = 1;
  y_pos[11] = 1;

  x_pos[12] = 1;
  y_pos[12] = 1;

  x_pos[13] = 1;
  y_pos[13] = 1;

  x_pos[14] = 1;
  y_pos[14] = 1;

  x_pos[15] = 1;
  y_pos[15] = 1;


  // Code For Load Cell Starts
  Serial.begin(9600);

  for (int i = 0, j = 2 ; i < num_of_scale ; i++) {
    scale[i].begin(j, j + 1);
    j += 2;
  }
  delay(5000);
  for (int i = 0 ; i < num_of_scale ; i++) {
    if (scale[i].is_ready()) {
      long reading = scale[i].read();
      Serial.print(i + 1);
      Serial.print(" no. scale is working. Its initial value is ");
      Serial.println(reading);
      init_val[i] = reading;
    }
    else {
      Serial.print(i + 1);
      Serial.println(" no. scale is not working properly. Check the connection. ");
    }
    delay(2000);
  }
  //AbsMouse.init(1920, 1080); //INPUT your screen resolution
  Mouse.begin();
  //MouseTo.setCorrectionFactor(1);
  //MouseTo.setMaxJump(5000);
  // while (MouseTo.move() == false) {}
  delay(3000);
  //MouseTo.home();
}

void loop() {
  for (int i = 0 ; i < num_of_scale ; i++) {
    if (scale[i].is_ready()) {
      long reading = scale[i].read();
      if (abs(reading - init_val[i]) > sensitivity) {
        // MouseTo.setTarget(x_pos[i], y_pos[i]);
        // while (MouseTo.move() == false) {}
        // Mouse.click(MOUSE_LEFT);

        Serial.print(i + 1);
        Serial.print(" no. scale is working. Its initial value is ");
        Serial.println(reading);

        //AbsMouse.move(x_pos[i], y_pos[i]);
        //AbsMouse.press(MOUSE_LEFT);
        delay(100);
        //AbsMouse.release(MOUSE_LEFT);
        Mouse.move(100,100,0);
        Mouse.click(MOUSE_LEFT);
      }
    }
    else {
      Serial.print(i + 1);
      Serial.println(" no. scale is not working properly. Check the connection. ");
    }
  }
  delay(500);
}
