#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>


// Leg 1
Servo coxa_servo1;
Servo femur_servo1;
Servo tibia_servo1;

// servo pins
const int coxa_pin = 13;
const int femur_pin = 12;
const int tibia_pin = 14;

// angles
double coxa_angle;
double femur_angle;
double tibia_angle;

// unknowns
float d;
float r;
float c;

// knowns
const float a = 80;
const float b = 140;
const float l = 67;

// positions
/*float x[] = {-50, -36, -12, 12, 36, 50};

float y[] = {75, 100, 100, 100, 100, 75};

float z[] = {-140, -100, -65, -65, -100, -140};

*/

float x[] = {
  -50, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5,
   0,   5,  10,  15,  20,  25,  30,  35,  40,  45, 50, 50
};

float y[] = {
  65, 65, 70, 75, 80, 85, 90, 95, 100, 100, 100,
  100, 100, 100, 100, 100, 100, 100, 95, 90, 85, 80, 80
};

float z[] = {
  -140, -140, -130, -120, -110, -105, -100, -95, -90, -85, -75,
  -65, -65, -65, -65, -65, -65, -65, -75, -85, -100, -140, -140
};

void setup() {
  // put your setup code here, to run once:
  coxa_servo1.attach(coxa_pin);
  femur_servo1.attach(femur_pin);
  tibia_servo1.attach(tibia_pin);
}

void loop() {
  // put your main code here, to run repeatedly:

  for (int i = 0; i < 23; i++) {
    
    d = sqrt(x[i] * x[i] + y[i] * y[i]);
    r = d - l;
    c = sqrt(r * r + z[i] * z[i]);

    // angles
    coxa_angle = atan2(y[i], x[i]) * 180 / M_PI;
    femur_angle = 180 - (atan2(r, -z[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    tibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12;
    
    coxa_servo.write(constrain(coxa_angle, 0, 180));
    femur_servo.write(constrain(femur_angle, 0, 180));
    tibia_servo.write(constrain(tibia_angle, 0, 180));
    delay(50);
}
}

