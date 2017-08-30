//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"

#include "pheatherstation.h"
#include "cactus_io_BME280_I2C.h"

#include <Wire.h>

#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

// HAL ////////////////////////////////////////////
const int LED_FLASH = D7; // Instead of writing D7 over and over again, we'll write led2. This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

int THERMISTOR = A3; // 10k thermistor
int ANEMOMETER = D3;
int WINDVANE = A0;
int RAINGAUGE = WKP;

double VCC = 3.33; // VCC voltage source
int ADC_MAX = 4096;

unsigned long DEBOUNCE_MS = 10;

SYSTEM_MODE(MANUAL);

// GLOBALS //////////////////////////////////////
PheatherStation gStation(VCC);

BME280_I2C gBMESensor(0x76);
bool bFoundSensor;
uint8_t chipID;

unsigned long gLastAnemometer = 0;
unsigned long gLastRain = 0;




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
	
	int digValue = analogRead(THERMISTOR);
	gStation.set_thermistor_voltage( digValue/(double)ADC_MAX * VCC ) ;
	
	stringstream msg;
	
	msg << "ambient temperature: " << gStation.get_temperature() << endl;
	
	msg << "wind speed: " << gStation.get_windspeed() << " avg: " << gStation.get_windspeed_avg()  << endl;
	
	gBMESensor.readSensor();
	float c_i2c = gBMESensor.getTemperature_C();
	float pres_mb = gBMESensor.getPressure_MB();
	float humidity = gBMESensor.getHumidity();
	
	msg  << "i2c temp: " << c_i2c << endl;

	msg << " pressure mb: " << pres_mb << endl;
	
	msg << " humidity: " << humidity << endl;
	
	debug_message(msg.str());	
	digValue = analogRead(WINDVANE);
	
	gStation.update_wind_data(micros());
	
	/*
	byte error, address;
	int nDevices;
	nDevices = 0;
	for(address = 1; address < 127; address++ )
	{
		if (address == 1)
		{
			debug_message("Scanning...");
			delay(5000);
		}
		// The i2c_scanner uses the return value of
		// the Write.endTransmisstion to see if
		// a device did acknowledge to the address.
		Wire.beginTransmission(address);
		error = Wire.endTransmission();

		stringstream msg;
		if (error == 0)
		{
			msg << "I2C device found at address ";
				
			msg << (int)address << " !";

			nDevices++;
		}
		else if (error==4)
		{
			msg << "Unknow error at address";
				
			msg << (int)address;
		}
		
		string msg_str = msg.str();
		if (!msg_str.empty())
		{
			debug_message(msg_str);
		}
		
		delay(100);
		debug_message(".");
	}
	
	if (nDevices == 0)
		debug_message("No I2C devices found\n");
	else
		debug_message("done\n");
	*/
	
}

void anemometer_interrupt()
{
	unsigned long now = micros();
	if (now - gLastAnemometer >= DEBOUNCE_MS * 1000)
	{
		gLastAnemometer = now;
		gStation.set_anemometer_turn(now);
		debug_message("anemometer revolution\n");
	}
	else
	{
		debug_message("anemometer bounce\n");
	}
}

void raingauge_interrupt()
{
	unsigned long now = micros();
	if (now - gLastRain >= DEBOUNCE_MS * 1000)
	{
		gLastRain = now;
		debug_message("rain gauage trigger\n");
	}
	else
	{
		debug_message("rain bounce\n");
	}
}