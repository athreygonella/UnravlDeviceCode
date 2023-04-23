// Code to test linear servo from Actuonix

#include <Arduino.h>
#include <Servo.h>
#include "LinearServo.h"

#define PIN_SERVO (5)

LinearServo myServo;

void setup() {
  Serial.begin(9600);
  myServo.attach(PIN_SERVO);
  Serial.println("Beginning Linear Servo Test");
}

void loop() {
  myServo.extend(myServo);
  Serial.println("Extended"); 
  delay(3000);
  myServo.retract(myServo);
  Serial.println("Retracted");
  delay(3000);
}
