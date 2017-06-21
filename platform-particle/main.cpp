//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"


int led1 = D0; // Instead of writing D0 over and over again, we'll write led1
// You'll need to wire an LED to this one to see it blink.

int led2 = D7; // Instead of writing D7 over and over again, we'll write led2
// This one is the little blue LED on your board. On the Photon it is next to D7, and on the Core it is next to the USB jack.

SYSTEM_MODE(MANUAL);

void setup() {
    WiFi.connect();
    waitUntil(WiFi.ready);
}

void loop() {
    digitalWrite(led2, HIGH);  // Turn ON the LED
	delay(1000);              // Wait for 1000mS = 1 second
	digitalWrite(led2, LOW);   // Turn OFF the LED
	delay(1000);              // Wait for 1 second
}
