/**
 * Main program to unravel hair
*/

#include <Arduino.h>
#include <Servo.h>
#include "BasicStepperDriver.h"
#include "LinearServo.h"

// For Stepper Motor
#define MOTOR_STEPS 200
#define RPM 600
#define MICROSTEPS 1
#define DIR 2
#define STEP 3
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// For Linear Servo
LinearServo myServo;
#define PIN_SERVO 8


void setup() {
  Serial.begin(9600);

  // Initialize motors
  stepper.begin(RPM, MICROSTEPS);
  myServo.attach(PIN_SERVO);
  myServo.retract(myServo);
  delay(3000);
  Serial.println("Motors successfully initialized");

  Serial.println("Beginning Unraveling"); 
  unravel();
}

void loop() {
// manually move start position
// stepper.rotate(360*10);
}

void unravel() {
  float lead = 0.2;         // cm

  for (float verticalStep = 1; verticalStep <= 30; verticalStep++) {
    
    // Move up to insertion location
    stepper.displaceLinear(verticalStep, lead);
    delay(1000);

    // Insert in
    myServo.extend(myServo);
    delay(3000);

    // Move down to bottom
    stepper.displaceLinear(-verticalStep, lead);
    delay(1000);

    // Insert out
    myServo.retract(myServo);
    delay(3000);
  }
}
