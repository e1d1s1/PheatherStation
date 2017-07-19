//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"


#include "pheatherstation.h"


class println;
int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2
// This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

SYSTEM_MODE(MANUAL);

PheatherStation gStation;

void debug_message(const string& msg)
{
	Serial.println(msg.c_str());
	Serial1.println(msg.c_str());
}

void setup() {
    //WiFi.connect();
    //waitUntil(WiFi.ready);
  	pinMode(led2, OUTPUT);
	
	Serial.begin(9600);
	Serial1.begin(9600);
	gStation.set_logger([=](const string& msg) { debug_message(msg); });
}

void loop() {
    digitalWrite(led2, HIGH);  // Turn ON the LED
	delay(1000);              // Wait for 1000mS = 1 second
	digitalWrite(led2, LOW);   // Turn OFF the LED
	delay(1000);              // Wait for 1 second
	
	//debug_message("test message");
}
