//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"

#include "pheatherstation.h"

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

void anemometer_interrupt();
void raingauge_interrupt();

void debug_message(const string& msg)
{
	Serial.println(msg.c_str());
	Serial1.println(msg.c_str());
}

void setup() {
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
}

void loop() {
    digitalWrite(LED_FLASH, HIGH);  // Turn ON the LED
	delay(1000);              // Wait for 1000mS = 1 second
	digitalWrite(LED_FLASH, LOW);   // Turn OFF the LED
	delay(1000);              // Wait for 1 second
	
	int digValue = analogRead(THERMISTOR);
	gStation.set_thermistor_voltage( digValue/(double)ADC_MAX * VCC ) ;
	
	char buffer[8];
	sprintf(buffer, "%f", gStation.get_temperature());
	
	string msg = "ambient temperature: ";
	msg += buffer;
	
	sprintf(buffer, "%d", digValue);
	msg += " ADC:";
	msg += buffer;
	msg += "\n";
	debug_message(msg);
	
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