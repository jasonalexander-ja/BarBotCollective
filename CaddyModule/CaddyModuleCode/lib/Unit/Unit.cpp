#include <Arduino.h>
#include <Wire.h>
#include <Unit.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include "Adafruit_VL53L0X.h"

#define STEPS 800
#define STEPS_PER_POS 250
#define CUP_DIST 150 

AccelStepper stepper(AccelStepper::MotorInterfaceType::DRIVER, 13, 12);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool loxOk = true;

void waitForCupPlaced();
void waitForCupRemoved();
void setZero();
int moveToPosition(int pos);

void unitSetup()
{
    stepper.setMaxSpeed(100);
    stepper.setAcceleration(20);
    Wire1.setPins(2, 15);
    loxOk = lox.begin(VL53L0X_I2C_ADDR, false, &Wire1);
}

int performAction(int option)
{
    if (!loxOk)
        return CaddyErrors::DistSensorInitError;

    switch (option)
    {
        case 255:
            waitForCupPlaced();
            break;
        case 254:
            waitForCupRemoved();
            break;
        case 253: 
            setZero();
            break;
        default:
            return moveToPosition(option);
    }

    // Return 0 when ran sucessfully 
    return 1;
}

int moveToPosition(int pos)
{
    int new_pos = pos * STEPS_PER_POS;
    
    delay(3000);
    //stepper.runToNewPosition(new_pos);
    return 1;
}

void setZero()
{
    stepper.setCurrentPosition(0l);
}

void waitForCupPlaced()
{
    VL53L0X_RangingMeasurementData_t measure;
    while (true)
    {
        lox.rangingTest(&measure, false);
        if (measure.RangeMilliMeter <= CUP_DIST)
        {
            delay(2000);
            return;
        }
        delay(200);
    }
}

void waitForCupRemoved()
{
    VL53L0X_RangingMeasurementData_t measure;
    while (true)
    {
        lox.rangingTest(&measure, false);
        if (measure.RangeMilliMeter >= CUP_DIST)
        {
            delay(2000);
            return;
        }
        delay(200);
    }
}

