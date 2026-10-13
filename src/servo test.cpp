
#include <Arduino.h>
#include <ESP32Servo.h>

Servo coxa_servo;
Servo femur_servo;
Servo tibia_servo;

const int coxa_pin = 13;
const int femur_pin = 12;
const int tibia_pin = 14;

float x, y, z;

void setup() {
  Serial.begin(115200);

  coxa_servo.attach(coxa_pin);
  femur_servo.attach(femur_pin);
  tibia_servo.attach(tibia_pin); u

  Serial.println("Enter angles as: X Y Z");
}

void loop() {
  if (Serial.available() > 0) {
    x = Serial.parseFloat();
    y = Serial.parseFloat();
    z = Serial.parseFloat();

    coxa_servo.write(constrain(x, 0, 180));
    femur_servo.write(constrain(y, 0, 180));
    tibia_servo.write(constrain(z, 0, 180));

    Serial.print("Coxa: ");
    Serial.println(x);

    Serial.print("Femur: ");
    Serial.println(y);

    Serial.print("Tibia: ");
    Serial.println(z);

    Serial.println("----------------");
  }
}
