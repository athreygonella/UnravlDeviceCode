/**
 * Logic for LinearServo class and its methods.
*/

#include "LinearServo.h"
#include "Servo.h"
#include "Arduino.h"

void LinearServo::retract(Servo s) {
    moveToPosMM(s, 0);
}

void LinearServo::extend(Servo s) {
    moveToPosMM(s, 25);
}

void LinearServo::moveToPosMM(Servo s, int length) {
    int maxStroke = 50; // (mm)
    if (length < 0) {
        length = 0;
    }
    if (length > maxStroke) {
        length = maxStroke;
    }

    float strokePercentage = ((float) length) / maxStroke;

    // Compute # of microseconds (ranges from 1000 - fully ccw to 2000 - fully cw)
    int microseconds = 1000 + strokePercentage * (2000 - 1000);
    
    s.writeMicroseconds(microseconds);
    
}
