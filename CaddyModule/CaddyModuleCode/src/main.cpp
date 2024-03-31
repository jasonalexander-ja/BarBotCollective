#include <Arduino.h>
#include <Wire.h>
#include <Unit.h>


/*
This is just utility setup code for communicating with the controller ext.

For programming your module please edit `Unit.cpp` in the `lib/Unit` folder, 
and ignore this file. 
*/

#define LED_BUILTIN 2

volatile uint8_t option = 0;
volatile bool isRunning = false;
volatile uint8_t lastRunResult = 1;

void receiveEvent(int howMany);
void requestEvent();

void setup()
{
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.begin(115200);
	unitSetup();
	Wire.begin(UNIT_ADDRESS, 27, 26, 0u);
	Wire.onReceive(receiveEvent);
	Wire.onRequest(requestEvent);
	Serial.println("Started");
}

void loop()
{
	if (isRunning)
	{
		Serial.print("Executing: ");
		Serial.println(String(option));
		
		delay(2000);
		//lastRunResult = performAction(option);
		isRunning = false;
		Serial.println("End. ");
		Serial.println("");
	}
}

void receiveEvent(int howMany)
{
	while (0 < Wire.available())
	{
		option = Wire.read();
	}
	Serial.print("Received ");
	Serial.println(String(option));
	isRunning = true;
}

void requestEvent()
{
	uint8_t result[4] = { 170, isRunning ? 1 : 2, lastRunResult, 170 };
	Wire.write(result, 4);
	Serial.print("Status: ");
	Serial.println(String(lastRunResult));
}
