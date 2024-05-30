#include <Arduino.h>
#include <Unit.h>


void runSequence(int pin);

void unitSetup()
{
    pinMode(6, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);

    digitalWrite(6, LOW);
    digitalWrite(7, LOW);
    digitalWrite(8, LOW);
    digitalWrite(9, LOW);
}

int performAction(int option)
{
    switch (option)
    {
        case 0:
            runSequence(6);
            break;
        case 1:
            runSequence(7);
            break;
        case 2:
            runSequence(8);
            break;
        case 3:
            runSequence(9);
            break;
    }
    return 0;
}


void runSequence(int pin)
{
    digitalWrite(pin, HIGH);
    delay(5000);
    digitalWrite(pin, LOW);
}

