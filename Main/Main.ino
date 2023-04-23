/**
 * Main program to unravel hair
*/

#include <Arduino.h>
#include <Servo.h>
#include "BasicStepperDriver.h"
#include "LinearServo.h"
#include <ezButton.h>
#include <LiquidCrystal.h>

// For Stepper Motor
#define MOTOR_STEPS 200
#define RPM 600
#define MICROSTEPS 1
#define DIR 3
#define STEP 2
#define SCREW_LEAD 0.2f   // cm
BasicStepperDriver stepper(MOTOR_STEPS, DIR, STEP);

// For Linear Servo
LinearServo myServo;
#define PIN_SERVO 5

// For Limit Switch
ezButton limitSwitch(19);

// For LCD screen
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);

// For Joystick
#define SW_PIN 4       // digital pin connected to switch output
#define X_PIN 11       // analog pin connected to X output
#define Y_PIN 10       // analog pin connected to Y output

float startHeight = 0;
float currentHeight;

void switchPressed() {
  Serial.println("Limit Switch: UNACTIVATED -> ACTIVATED");
  stepper.stop();
}

void setup() {
  Serial.begin(9600);

  /*
   * Initialization
  */

  //Initialize limit switch 
  limitSwitch.setDebounceTime(50);
  limitSwitch.loop(); // MUST call the loop() function first
  attachInterrupt(digitalPinToInterrupt(19), switchPressed, RISING);

  // Initialize stepper motor and lead screw
  stepper.begin(RPM, MICROSTEPS);
  delay(1000);

  // Initialize Analog Joystick
  pinMode(SW_PIN, INPUT);
  digitalWrite(SW_PIN, HIGH);

  // Initialize LCD screen
  lcd.begin(16, 2);

  Serial.println("Initialization complete!"); 

  /*
   * Begin Main Flow
  */
  
  // State 1) Press to Begin
  lcd.print("Press to Begin");
  bool joystickPressed = false;
  
  while (!joystickPressed) {
    if (digitalRead(SW_PIN) == LOW) {
      joystickPressed = true;
      lcd.clear();
      digitalWrite(SW_PIN, HIGH);
      delay(200);
    }
    delay(50);
  }

  // State 2) Set Starting Height
  lcd.print("Set Starting");
  lcd.setCursor(0, 1);  // Go to next line
  lcd.print("Height: ");
  joystickPressed = false;
  
  unsigned long previousSetHeightMillis = millis();
  unsigned long previousBlinkMillis = millis();
  bool numShown = false;
  
  while (!joystickPressed) {

    if (digitalRead(SW_PIN) == LOW) {
      joystickPressed = true;
      lcd.clear();
      digitalWrite(SW_PIN, HIGH);
      delay(200);
      break;
    }

    const long setHeightInterval = 200;
    unsigned long currentSetHeightMillis = millis();
    if (currentSetHeightMillis - previousSetHeightMillis >= setHeightInterval) {
      previousSetHeightMillis = currentSetHeightMillis;
      setStartingHeight();
    }
    
    
    // Blink logic
    const long blinkInterval = 500;
    unsigned long currentBlinkMillis = millis();
    if (currentBlinkMillis - previousBlinkMillis >= blinkInterval) {
      // It's time to blink
      previousBlinkMillis = currentBlinkMillis; // Save the last time it blinked
      if (!numShown) {
        lcd.setCursor(8, 1);
        lcd.print(startHeight);
        numShown = true;
      } else {
        lcd.setCursor(8, 1);
        lcd.print("    ");
        numShown = false;
      }
    }
  }


  // State 3) Press to Unravel
  lcd.print("Press to Unravel");
  lcd.setCursor(0, 1);  // Go to next line
  lcd.print("Braid");
  joystickPressed = false;
  
  while (!joystickPressed) {
    if (digitalRead(SW_PIN) == LOW) {
      joystickPressed = true;
      lcd.clear();
      delay(500);
      break;
    }
  }


  // State 4) Unravel (after resetting)
  lcd.print("Unraveling...");
  Serial.println("Beginning Unraveling");

  // Reset tine so it's retracted and at bottom

  // Initialize linear servo
  myServo.attach(PIN_SERVO);
  myServo.retract(myServo);
  delay(3000);

  // go down IF switch is not pressed
  limitSwitch.loop();
  int state = limitSwitch.getState();
  if (state == HIGH) {
    Serial.println("The limit switch: UNTOUCHED");
    float resetLowering = -100;   // TODO: make it rotate infinity down once limit switch is mechanically attached
    stepper.displaceLinear(resetLowering, SCREW_LEAD);
    delay(200);
  } else {
    Serial.println("The limit switch: TOUCHED");
    delay(200);
  }
  
  stepper.displaceLinear(0.2, SCREW_LEAD);  // just to move away from limit switch a lil bit
  
  float offset = 0.6;
  startHeight = startHeight * 2.54; // convert startHeight from in to cm
  delay(500);
  stepper.displaceLinear(startHeight + offset, SCREW_LEAD);  
  currentHeight = startHeight + offset + 6;

  unravel();


  // State 5) Success
  lcd.clear();
  lcd.print("Success!");
  lcd.setCursor(0, 1);  // Go to next line
  lcd.print("Unravel Complete");
  
}

void loop() {
}

void setStartingHeight() {
  int x_pos = analogRead(X_PIN);
  int y_pos = analogRead(Y_PIN);

  if (y_pos > 800) {
    // Decrement startHeight (in)
    if (startHeight != 0) {
      startHeight = startHeight - 0.5;
    }
  }

  if (y_pos < 100) {
    // Increment startHeight (in)
    if (startHeight != 30) {
      startHeight = startHeight + 0.5;
    }
  }
}

void unravel() {
 
  delay(1000);

  float verticalStep = 0.5; // cm
  float topLimit = 30;      // cm

  for (float insertionHeight = verticalStep; currentHeight <= topLimit; insertionHeight += verticalStep) {

    currentHeight += verticalStep;

    Serial.println(currentHeight);
    
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
