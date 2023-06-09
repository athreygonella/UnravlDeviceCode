/*
 * Simple demo, should work with any driver board
 *
 * Connect STEP, DIR as indicated
 *
 * Copyright (C)2015-2017 Laurentiu Badea
 *
 * This file may be redistributed under the terms of the MIT license.
 * A copy of this license has been included with this distribution in the file LICENSE.
 */
#include <Arduino.h>
#include "BasicStepperDriver.h"

// Motor steps per revolution. Most steppers are 200 steps or 1.8 degrees/step
#define MOTOR_STEPS 200

#define RPM 500   // Change to modify speed

// Since microstepping is set externally, make sure this matches the selected mode
// If it doesn't, the motor will move at a different RPM than chosen
// 1=full step, 2=half step etc.
#define MICROSTEPS 1

// All the wires needed for full functionality
#define DIR 3
#define STEP 2
//Uncomment line to use enable/disable functionality
//#define SLEEP 13

// 2-wire basic config, microstepping is hardwired on the driver
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

//Uncomment line to use enable/disable functionality
//BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP, SLEEP);

void setup() {
    stepper.begin(RPM, MICROSTEPS);
    // if using enable/disable on ENABLE pin (active LOW) instead of SLEEP uncomment next line
    // stepper.setEnableActiveState(LOW);
    delay(1000);
}

void loop() {
  
    // energize coils - the motor will hold position
    // stepper.enable();

//    float verticalStep = 5;   // cm
//    float lead = 0.2;  // cm
//    float revolutions = verticalStep/lead;
  
    /*
     * Moving motor one full revolution using the degree notation
     */
    stepper.rotate(360*6);
    //stepper.displaceLinear(-verticalStep, lead);

    /*
     * Moving motor to original position using steps
     */
    delay(1000);
    //stepper.move(MOTOR_STEPS*MICROSTEPS*4);
    stepper.rotate(-360*6);
    // stepper.displaceLinear(-verticalStep, lead);

    // pause and allow the motor to be moved by hand
    // stepper.disable();

    delay(1000);
}
