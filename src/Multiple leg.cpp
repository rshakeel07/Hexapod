// All Libraries
#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

// Board settings

#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz update rate
// Left servos
const int coxa_pin[] = {0, 3, 6}; // MAYBE CHANGE to 012 345 678 depending on servo layout 
const int femur_pin[] = {1, 4, 7}; // right now it is, each legs 3 servos are in order together, in order of coxa, femur, tibia, then the next leg starts with coxa again
const int tibia_pin[] = {2, 5, 8};

// Servo board
Adafruit_PWMServoDriver leftpwm = Adafruit_PWMServoDriver();
Adafruit_PWMServoDriver rightpwm = Adafruit_PWMServoDriver(0x41);
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
  Serial.begin(115200);
  leftpwm.begin();
  leftpwm.setPWMFreq(SERVO_FREQ);
  leftpwm.setOscillatorFrequency(27000000);

  rightpwm.begin();
  rightpwm.setPWMFreq(SERVO_FREQ);
  rightpwm.setOscillatorFrequency(27000000);
  Wire.begin();
}

void loop() {
  // loop for all coordinate points in the arrays (x, y, z)
  for (int i = 0; i < 23; i++) {

    // IK variables
    d = sqrt(x[i] * x[i] + y[i] * y[i]);
    r = d - l;
    c = sqrt(r * r + z[i] * z[i]);

    // IK angles
    coxa_angle = atan2(y[i], x[i]) * 180 / M_PI;
    femur_angle = 180 - (atan2(r, -z[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    tibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12; // -12 is the offset since it isnt perfectly straight when the servo is at 90 degrees

    // Writing to angles
    for (int j = 0; j < 3; j++)
    {
        leftpwm.setPWM(coxa_pin[j], 0, map(coxa_angle, 0, 180, SERVOMIN, SERVOMAX));
        rightpwm.setPWM(femur_pin[j], 0, map(femur_angle, 0, 180, SERVOMIN, SERVOMAX));
        leftpwm.setPWM(tibia_pin[j], 0, map(tibia_angle, 0, 180, SERVOMIN, SERVOMAX));
        delay(50);
    }
    delay(1000);
    for (int k = 0; k < 3; k++)
    {
        rightpwm.setPWM(coxa_pin[k], 0, map(coxa_angle, 0, 180, SERVOMIN, SERVOMAX));
        leftpwm.setPWM(femur_pin[k], 0, map(femur_angle, 0, 180, SERVOMIN, SERVOMAX));
        rightpwm.setPWM(tibia_pin[k], 0, map(tibia_angle, 0, 180, SERVOMIN, SERVOMAX));
    }
    delay(30);
}
}