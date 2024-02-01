#include <Arduino.h>
#include <Wire.h>
#include <Unit.h>
#include <Stepper.h>
#include "Adafruit_VL53L0X.h"

/*
This is where all your code should live for your module. 

Place all code you want to execute on startup in `void unitSetup()`, 
this is like `setup()` when programming regular Arduino. 

The code you want to run when the control unit requests an action should 
live inside `int performAction(int option)`. 

All Arduino functions and concepts will work here.  

If you have a simple unit that say moves a servo back and forth to 
dispense a shot, you can just add the code to do this and add the 
`return 0` at the end. 

Here's an example that just flashes an LED: 

int performAction(int option)
{
    digitalWrite(led1, HIGH);
    delay(500);
    digitalWrite(led1, LOW);
    delay(500);
    return 0;
}

If your module can do multiple different things, such as light up different 
LEDs, we can assign each of these a number between 0 and 255 in the 
control unit, you will then receive the number in the `option` parameter 
that is passed into the `performAction` function. 

Example: 

int performAction(int option)
{
    switch(option)
    {
        case 1: 
            digitalWrite(led1, HIGH);
            break;
        case 2: 
            digitalWrite(led2, HIGH);
            break;
        default: 
            digitalWrite(led1, HIGH);
            digitalWrite(led2, HIGH);
    }
    return 0;
}

If you want to communicate back an error to the control unit, you can simply 
return a number between 1 and 255 from the `performAction` function (0 means
no error/sucessful). 

Here's an example: 

int performAction(int option)
{
    int temperature = analogRead(ADC_TEMP);

    if (temperature > 900)
    {
        return 3; // Too hot! 
    }

    return 0;
}

Once a return is hit in `performAction` function, no more code will be executed
until another request is made, it's just jumping to the end of the function. 

Happy Hacking! 
*/

#define STEPS 800

Stepper stepper(STEPS, 12, 13);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool loxOk = false;

void unitSetup()
{
    // Put your code in here to run once on setup 
    loxOk = lox.begin(VL53L0X_I2C_ADDR, false, &Wire1);
    Wire1.setPins(33, 32);
}

int performAction(int option)
{
    // Put your code here to run when an action is requested 
    if (!loxOk)
        return CaddyErrors::DistSensorInitError;
        
    // Return 0 when ran sucessfully 
    return 0;
}

