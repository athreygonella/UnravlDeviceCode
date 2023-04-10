/**
 * Defines LinearServo class and its methods.
*/
#ifndef LINEARSERVO_H
#define LINEARSERVO_H
#include <Servo.h>

class LinearServo : public Servo {
public:
    void retract(Servo s);
    void extend(Servo s);

private:
    void moveToPosMM(Servo s, int length);
};

#endif /* LINEARSERVO_H */