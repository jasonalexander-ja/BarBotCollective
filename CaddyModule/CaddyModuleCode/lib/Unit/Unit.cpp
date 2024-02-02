#include <Arduino.h>
#include <Wire.h>
#include <Unit.h>
#include <Stepper.h>
#include <AccelStepper.h>
#include "Adafruit_VL53L0X.h"

#define STEPS 800
#define MAX_RANGE 1000 
#define MM_PER_POSITION 100
#define VARIATION 10

int zeroOffset = 0;

AccelStepper stepper(AccelStepper::MotorInterfaceType::DRIVER, 12, 13);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool loxOk = false;

void waitForCupPlaced();
void waitForCupRemoved();
void setZeroOffset();
int moveToPosition(int pos);
int getDistance();

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
            setZeroOffset();
            break;
        default:
            return moveToPosition(option);
    }

    // Return 0 when ran sucessfully 
    return 0;
}

int moveToPosition(int pos)
{
    int target = (pos * MM_PER_POSITION) - (MM_PER_POSITION / 2); 
    return 0;
}

int getDistance()
{
    VL53L0X_RangingMeasurementData_t measure;
    for (int i = 0; i < 10; i++)
    {
        lox.rangingTest(&measure, false);
        if (measure.RangeStatus != 4 && 
            measure.RangeMilliMeter - zeroOffset <= MAX_RANGE)
        {
            return measure.RangeMilliMeter - zeroOffset;
        }
    }
    return -1;
}

void setZeroOffset()
{
    VL53L0X_RangingMeasurementData_t measure;
    lox.rangingTest(&measure, false);
    if (measure.RangeStatus != 4 && measure.RangeMilliMeter <= MAX_RANGE)
    {
        zeroOffset = measure.RangeMilliMeter;
    }
}

void waitForCupPlaced()
{
    VL53L0X_RangingMeasurementData_t measure;
    while (true)
    {
        lox.rangingTest(&measure, false);
        if (measure.RangeMilliMeter == 4 || measure.RangeMilliMeter > MAX_RANGE)
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
        if (measure.RangeMilliMeter != 4 && measure.RangeMilliMeter <= MAX_RANGE)
        {
            delay(2000);
            return;
        }
        delay(200);
    }
}

