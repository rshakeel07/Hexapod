#include <Arduino.h>
#include <ESP32Servo.h>
#include <math.h>

Servo coxa_servo;
Servo femur_servo;
Servo tibia_servo;

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
float x;
float y;
float z;



void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Enter as: X Y Z");
  coxa_servo.attach(coxa_pin);
  femur_servo.attach(femur_pin);
  tibia_servo.attach(tibia_pin);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    
    // Read the three numbers in sequence
    x = Serial.parseFloat();
    y = Serial.parseFloat();
    z = Serial.parseFloat();
    delay(10);
    while (Serial.available() > 0) {
      Serial.read(); // Clear the buffer
    }

    d = sqrt(x * x + y * y);
    r = d - l;
    c = sqrt(r * r + z * z);

    // angles
    coxa_angle = atan2(y, x) * 180 / M_PI;
    femur_angle = 180 - (atan2(r, -z) * 180 / M_PI + acos ((a * a + c * c - b * b) / (2 * a * c)) * 180 / M_PI);
    tibia_angle = acos((a * a + b * b - c * c) / (2 * a * b)) * 180 / M_PI - 12;

    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println("-----------------------");
    Serial.print("Coxa Angle: ");
    Serial.println(coxa_angle);
    Serial.print("Femur Angle: ");
    Serial.println(femur_angle);
    Serial.print("Tibia Angle: ");
    Serial.println(tibia_angle);
    
    coxa_servo.write(constrain(coxa_angle, 0, 180));
    femur_servo.write(constrain(femur_angle, 0, 180));
    tibia_servo.write(constrain(tibia_angle, 0, 180));
}
}
