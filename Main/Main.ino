/**
 * Main program to unravel hair
*/

#include <Arduino.h>
#include <Servo.h>
#include "BasicStepperDriver.h"
#include "LinearServo.h"
#include <ezButton.h>

// For Stepper Motor
#define MOTOR_STEPS 200
#define RPM 600
#define MICROSTEPS 1
#define DIR 9
#define STEP 10
#define SCREW_LEAD 0.2f   // cm
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// For Linear Servo
LinearServo myServo;
#define PIN_SERVO 8

// For Limit Switch
ezButton limitSwitch(2);

void switchPressed() {
  Serial.println("Limit Switch: UNACTIVATED -> ACTIVATED");
  stepper.stop();
}

void setup() {
  Serial.begin(9600);

  //Initialize limit switch 
  limitSwitch.setDebounceTime(50);
  limitSwitch.loop(); // MUST call the loop() function first
  attachInterrupt(digitalPinToInterrupt(2), switchPressed, RISING);

  // Initialize stepper motor and lead screw
  stepper.begin(RPM, MICROSTEPS);
  delay(100);

  float resetLowering = -3;
  stepper.displaceLinear(resetLowering, SCREW_LEAD);   // TODO: make it rotate infinity down once limit switch is mechanically attached

  // Initialize linear servo
  myServo.attach(PIN_SERVO);
  myServo.retract(myServo);
  delay(3000);

  Serial.println("Initialization complete!");  

  // Begin unraveling
  Serial.println("Beginning Unraveling"); 
  unravel();
}

void loop() {
  // to manually move start position (delete once limit switch is mechanically attached):
  // stepper.rotate(360*30);
}

void unravel() {

  float verticalStep = 0.5;

  for (float insertionHeight = verticalStep; insertionHeight <= 30; insertionHeight += verticalStep) {
    
    // Move up to insertion location
    stepper.displaceLinear(insertionHeight, SCREW_LEAD);
    delay(1000);

    // Insert in
    myServo.extend(myServo);
    delay(3000);

    // Move down to bottom
    stepper.displaceLinear(-insertionHeight, SCREW_LEAD);
    delay(1000);

    // Insert out
    myServo.retract(myServo);
    delay(3000);
  }
}
