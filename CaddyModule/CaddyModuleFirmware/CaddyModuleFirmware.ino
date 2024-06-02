#include <Stepper.h>
#include <AccelStepper.h>
#include "Adafruit_VL53L0X.h"

#define STEPS 800
#define STEPS_PER_POS 500
#define CUP_DIST 150 

#define UNIT_ADDRESS 18

AccelStepper stepper(AccelStepper::MotorInterfaceType::DRIVER, 13, 12);

Adafruit_VL53L0X lox = Adafruit_VL53L0X();
bool loxOk = true;

volatile uint8_t option = 0;
volatile bool isRunning = false;
volatile uint8_t lastRunResult = 0;

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	unitSetup();
  
	Wire1.begin(UNIT_ADDRESS);
	Wire1.onReceive(receiveEvent);
	Wire1.onRequest(requestEvent);
  Serial.begin(115200);
}

void loop()
{
  digitalWrite(LED_BUILTIN, HIGH);
  VL53L0X_RangingMeasurementData_t measure;
  Serial.print("Measured ");
  lox.rangingTest(&measure, false);
  Serial.println(String(measure.RangeMilliMeter));
	if (isRunning)
	{
		lastRunResult = performAction(option);
		isRunning = false;
	}
  delay(50);
  digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}

void receiveEvent(int howMany)
{
	while (0 < Wire1.available())
	{
		option = Wire1.read();
	}
	isRunning = true;
}

void requestEvent()
{
	uint8_t result[4] = { !isRunning, lastRunResult };
	Wire1.write(result, 4);
}

void unitSetup()
{
  stepper.setMaxSpeed(6000);
  stepper.setAcceleration(720);
  loxOk = lox.begin(VL53L0X_I2C_ADDR, false, &Wire);
}

typedef enum {
    DistSensorInitError = 1
} CaddyErrors;

int performAction(int option)
{

  int res = 0;

  switch (option)
  {
    case 255:
      res = waitForCupPlaced();
      break;
    case 254:
      res = waitForCupRemoved();
      break;
    case 253: 
      setZero();
      break;
    default:
      res = moveToPosition(option);
  }

  // Return 0 when ran sucessfully 
  return res;
}

int moveToPosition(int pos)
{
  int new_pos = pos * STEPS_PER_POS;
  
  stepper.runToNewPosition(new_pos);
  return 0;
}

void setZero()
{
  stepper.setCurrentPosition(0l);
}

int waitForCupPlaced()
{
  if (!loxOk)
    return CaddyErrors::DistSensorInitError;
  VL53L0X_RangingMeasurementData_t measure;
  while (true)
  {
    lox.rangingTest(&measure, false);
    if (measure.RangeMilliMeter <= CUP_DIST)
    {
      delay(2000);
      return 0;
    }
    delay(200);
  }
}

int waitForCupRemoved()
{
  if (!loxOk)
    return CaddyErrors::DistSensorInitError;
  VL53L0X_RangingMeasurementData_t measure;
  while (true)
  {
    Serial.print("Measured ");
    lox.rangingTest(&measure, false);
    Serial.println(String(measure.RangeMilliMeter));
    if (measure.RangeMilliMeter >= CUP_DIST)
    {
      delay(2000);
      return 0;
    }
    delay(200);
  }
}

