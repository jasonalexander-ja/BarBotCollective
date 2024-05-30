#include <Arduino.h>
#include <Unit.h>
#include <Servo.h>

Servo servo1;  // create servo object to control a servo
Servo servo2;
Servo servo3;
Servo servo4;

#define Servo1Down 90
#define Servo1Up 20

#define Servo2Down 27
#define Servo2Up 95

#define Servo3Down 20
#define Servo3Up 90

#define Servo4Down 85
#define Servo4Up 20

void runSequence(Servo& servo, int up, int down);

void unitSetup()
{
    servo1.attach(6, 500, 2500);
    servo2.attach(7, 500, 2500);
    servo3.attach(8, 500, 2500);
    servo4.attach(9, 500, 2500);
    delay(20);
    servo1.write(Servo1Down);
    servo2.write(Servo2Down);
    servo3.write(Servo3Down);
    servo4.write(Servo4Down);
}

int performAction(int option)
{
    switch (option)
    {
        case 0:
            runSequence(servo1, Servo1Up, Servo1Down);
            break;
        case 1:
            runSequence(servo2, Servo2Up, Servo2Down);
            break;
        case 2:
            runSequence(servo4, Servo4Up, Servo4Down);
            break;
        case 3:
            runSequence(servo3, Servo3Up, Servo3Down);
            break;
        case 5:
            servo1.write(Servo1Down);
            servo2.write(Servo2Down);
            servo3.write(Servo3Down);
            servo4.write(Servo4Down);
            break;
    }
    return 0;
}

void runSequence(Servo& servo, int up, int down)
{
    servo.write(up);
    delay(2000);
    servo.write(down);
    delay(500);
}

