#include <Arduino.h>
#include <Wire.h>
#include <Unit.h>


/*
This is just utility setup code for communicating with the controller ext.

For programming your module please edit `Unit.cpp` in the `lib/Unit` folder, 
and ignore this file. 
*/

#define LED_BUILTIN 2

uint8_t option = 0;
bool isRunning = false;
uint8_t lastRunResult = 0;

void receiveEvent(int howMany);
void requestEvent();

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	unitSetup();
	Wire.begin(UNIT_ADDRESS, 27, 26);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);
}

void loop()
{
	digitalWrite(LED_BUILTIN, HIGH);
	delay(50);
	if (isRunning)
	{
		lastRunResult = performAction(option);
		isRunning = false;
	}
	digitalWrite(LED_BUILTIN, LOW);
	delay(50);
}

void receiveEvent(int howMany)
{
	while (0 < Wire.available())
	{
		option = Wire.read();
	}
	isRunning = true;
}

void requestEvent()
{
	uint8_t result[2] = { !isRunning, lastRunResult };
	Wire.write(result, 2);
}
