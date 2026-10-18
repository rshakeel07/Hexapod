// All Libraries
#include <Arduino.h>
#include <math.h>
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

#define TIME_BETWEEN_CYCLES 30 // milliseconds
#define CYCLE_TIME 690 // milliseconds

// Board settings

#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  500 // This is the 'maximum' pulse length count (out of 4096)
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz update rate
// Left servos
const int coxa_pin[] = {0, 4}; // MAYBE CHANGE to 012 345 678 depending on servo layout 
const int femur_pin[] = {1, 5}; // right now it is, each legs 3 servos are in order together, in order of coxa, femur, tibia, then the next leg starts with coxa again
const int tibia_pin[] = {2, 6};

// Servo board
Adafruit_PWMServoDriver leftpwm = Adafruit_PWMServoDriver();

// return and arc angles
double rcoxa_angle;
double rfemur_angle;
double rtibia_angle;

double acoxa_angle;
double afemur_angle;  
double atibia_angle;

// unknowns
float d;
float r;
float c;

// knowns
const float a = 80;
const float b = 140;
const float l = 67;

// Arc positions
float xarc[] = {
  -50, -50, -45, -40, -35, -30, -25, -20, -15, -10, -5,
    0,   5,  10,  15,  20,  25,  30,  35,  40,  45, 50, 50
};

float yarc[] = {
   80,  80,  85,  90,  95, 100, 105, 110, 115, 120, 125,
  130, 130, 130, 130, 130, 130, 130, 125, 120, 110, 90, 80
};

float zarc[] = {
  -140, -140, -130, -120, -110, -105, -100, -95, -90, -85, -75,
   -65,  -65,  -65,  -65,  -65,  -65,  -65,  -75, -85, -100, -140, -140
};
// Return positions

float return_x[] = {
   50,  45.45,  40.91,  36.36,  31.82,  27.27,
   22.73, 18.18, 13.64, 9.09, 4.55, 0,
   -4.55, -9.09, -13.64, -18.18, -22.73,
   -27.27, -31.82, -36.36, -40.91, -45.45, -50
};

float return_y[] = {
  80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80,
  80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80, 80
};

float return_z[] = {
  -140, -140, -140, -140, -140, -140, -140, -140,
  -140, -140, -140, -140, -140, -140, -140, -140,
  -140, -140, -140, -140, -140, -140, -140
};




void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  leftpwm.begin();
  leftpwm.setPWMFreq(SERVO_FREQ);
  leftpwm.setOscillatorFrequency(27000000);
  Wire.begin();
}

void loop() {
  // loop for legs 1, 3, 5 arcing and 2, 4, 6 returning
  for (int i = 0; i < 23; i++) {

    // IK variables for arc
    d = sqrt(xarc[i] * xarc[i] + yarc[i] * yarc[i]);
    r = d - l;
    c = sqrt(r * r + zarc[i] * zarc[i]);

    // IK angles for arc
    acoxa_angle = atan2(yarc[i], xarc[i]) * 180 / M_PI;
    afemur_angle = 180 - (atan2(r, -zarc[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    atibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12; // -12 is the offset since it isnt perfectly straight when the servo is at 90 degrees

    // IK variables for return
    d = sqrt(return_x[i] * return_x[i] + return_y[i] * return_y[i]);
    r = d - l;
    c = sqrt(r * r + return_z[i] * return_z[i]);

    // IK angles for return
    rcoxa_angle = atan2(return_y[i], return_x[i]) * 180 / M_PI;
    rfemur_angle = 180 - (atan2(r, -return_z[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    rtibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12; // -12 is the offset since it isnt perfectly straight when the servo is at 90 degrees

    // Writing to angles

    leftpwm.setPWM(coxa_pin[0], 0, map(rcoxa_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(femur_pin[0], 0, map(rfemur_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(tibia_pin[0], 0, map(rtibia_angle, 0, 180, SERVOMIN, SERVOMAX));

    leftpwm.setPWM(coxa_pin[1], 0, map(acoxa_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(femur_pin[1], 0, map(afemur_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(tibia_pin[1], 0, map(atibia_angle, 0, 180, SERVOMIN, SERVOMAX));
    delay(CYCLE_TIME / 23); // delay for the cycle time divided by the number of points in the arrays
  }


  delay(TIME_BETWEEN_CYCLES); // wait for a bit before starting the next cycle
// loop for legs 2, 4, 6 arcing and 1, 3, 5 returning
  for (int i = 0; i < 23; i++) {

    // IK variables for arc
    d = sqrt(xarc[i] * xarc[i] + yarc[i] * yarc[i]);
    r = d - l;
    c = sqrt(r * r + zarc[i] * zarc[i]);

    // IK angles for arc
    acoxa_angle = atan2(yarc[i], xarc[i]) * 180 / M_PI;
    afemur_angle = 180 - (atan2(r, -zarc[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    atibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12; // -12 is the offset since it isnt perfectly straight when the servo is at 90 degrees

    // IK variables for return
    d = sqrt(return_x[i] * return_x[i] + return_y[i] * return_y[i]);
    r = d - l;
    c = sqrt(r * r + return_z[i] * return_z[i]);

    // IK angles for return
    rcoxa_angle = atan2(return_y[i], return_x[i]) * 180 / M_PI;
    rfemur_angle = 180 - (atan2(r, -return_z[i]) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    rtibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12; // -12 is the offset since it isnt perfectly straight when the servo is at 90 degrees

    // Writing to angles
    leftpwm.setPWM(coxa_pin[0], 0, map(acoxa_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(femur_pin[0], 0, map(afemur_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(tibia_pin[0], 0, map(atibia_angle, 0, 180, SERVOMIN, SERVOMAX));

    leftpwm.setPWM(coxa_pin[1], 0, map(rcoxa_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(femur_pin[1], 0, map(rfemur_angle, 0, 180, SERVOMIN, SERVOMAX));
    leftpwm.setPWM(tibia_pin[1], 0, map(rtibia_angle, 0, 180, SERVOMIN, SERVOMAX));

    delay(CYCLE_TIME / 23);
}
}