//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"

#include "pheatherstation.h"
#include "cactus_io_BME280_I2C.h"

#include <Wire.h>

#include <iostream>
#include <string>
#include <stdio.h>


class println;

const int LED_FLASH = D7; // Instead of writing D7 over and over again, we'll write led2. This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

int THERMISTOR = A3; // 10k thermistor
int ANEMOMETER = D3;
int WINDVANE = A0;
int RAINGAUGE = WKP;

double VCC = 3.33; // VCC voltage source
int ADC_MAX = 4096;

SYSTEM_MODE(MANUAL);

PheatherStation gStation(VCC);
BME280_I2C gBMESensor;
bool bFoundSensor;
uint8_t chipID;

void anemometer_interrupt();
void raingauge_interrupt();

void debug_message(const string& msg)
{
	Serial.println(msg.c_str());
	Serial1.println(msg.c_str());
}

void setup() {
	chipID = 0;
	
    //WiFi.connect();
    //waitUntil(WiFi.ready);
  	pinMode(LED_FLASH, OUTPUT);
	
	pinMode(THERMISTOR, INPUT);
	pinMode(WINDVANE, INPUT);
	
	pinMode(ANEMOMETER, INPUT);
	attachInterrupt(ANEMOMETER, anemometer_interrupt, RISING);
	
	pinMode(RAINGAUGE, INPUT);
	attachInterrupt(RAINGAUGE, raingauge_interrupt, RISING);
	
	Serial.begin(9600);
	Serial1.begin(9600);
	gStation.set_logger([=](const string& msg) { debug_message(msg); });
	
	bFoundSensor = gBMESensor.begin(chipID);
}

void loop() {
    digitalWrite(LED_FLASH, HIGH);  // Turn ON the LED
	delay(1000);              // Wait for 1000mS = 1 second
	digitalWrite(LED_FLASH, LOW);   // Turn OFF the LED
	delay(1000);              // Wait for 1 second
	
	if (bFoundSensor)
	{
		debug_message("found BME280");
	}
	else
	{
		debug_message("Could not find BME280");
		char buffer[8];
		sprintf(buffer, "%u", chipID);
		debug_message(buffer);
	}
	
	int digValue = analogRead(THERMISTOR);
	gStation.set_thermistor_voltage( digValue/(double)ADC_MAX * VCC ) ;
	
	char buffer[16];
	//sprintf(buffer, "%f", gStation.get_temperature());
	
	//string msg = "ambient temperature: ";
	//msg += buffer;
	//debug_message(msg);
	
	
	gBMESensor.readSensor();
	float c_i2c = gBMESensor.getTemperature_C();
	float pres_mb = gBMESensor.getPressure_MB();
	float humidity = gBMESensor.getHumidity();
	memset(buffer, '\0', 16);
	sprintf(buffer, "%f", c_i2c);
	string msg  = "i2c temp: ";
	msg += buffer;
	debug_message(msg);
	/*
	memset(buffer, '\0', 16);
	sprintf(buffer, "%f", pres_mb);
	msg = " pressure mb: ";
	msg += buffer;
	debug_message(msg);
	memset(buffer, '\0', 16);
	sprintf(buffer, "%f", humidity);
	msg = " humidity: ";
	msg += buffer;
	debug_message(msg);
	*/
	
	digValue = analogRead(WINDVANE);
	
}

void anemometer_interrupt()
{
	debug_message("anemometer revolution\n");
}

void raingauge_interrupt()
{
	debug_message("rain gauage trigger\n");
}