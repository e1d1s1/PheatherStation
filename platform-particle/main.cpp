//#include "vive_sensors_pipeline.h"
//#include "settings.h"
#include "application.h"

#include "pheatherstation.h"
#include "eeprom_settings.h"
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
int SETUP = D4;

double VCC = 3.33; // VCC voltage source
int ADC_MAX = 4096;

unsigned long DEBOUNCE_MS = 10;


SYSTEM_MODE(MANUAL);

// GLOBALS //////////////////////////////////////
PheaterStation_Settings g_settings;

PheatherStation g_station(VCC);
BME280_I2C g_BMESensor(0x76);

TCPServer restful_server = TCPServer(80);
TCPClient restful_client;

bool g_found_sensor;
uint8_t g_chipID;
unsigned long g_last_anemometer = 0;
unsigned long g_last_rain = 0;
bool g_setup_mode = false;




void anemometer_interrupt();
void raingauge_interrupt();
void setup_interrupt();

void debug_message(const string& msg)
{
	Serial.println(msg.c_str());
	Serial1.println(msg.c_str());
}

void connect_to_wifi(const char* ssid, const char* pass)
{
	WiFi.disconnect();
	WiFi.on();
	WiFi.setCredentials(ssid, pass);
	WiFi.connect();
	while (!WiFi.ready())
	{
		delay(500);
		debug_message("wiating on connection to ");
		debug_message(ssid);
	}
	debug_message("connected");
}

PheaterStation_Settings read_eeeprom_settings()
{
	PheaterStation_Settings retval;
	
	EEPROM.get(0, retval);
	
	/*
	{
		retval.first_boot = 1;
		strcpy(retval.wifi_name, "BACHELORPAD-WAP");
		strcpy(retval.wifi_pass, "490c9604ac333ef61aca123e33");
		EEPROM.put(0, retval);
	} */
	
	return retval;
};

void wait_for_serial_interrupt()
{
	//if USB is connected
}

bool do_serial_network_settings(PheaterStation_Settings& settings)
{
	bool bChanged = false;
	debug_message("\nEnter new network name:\n");
	Serial.flush();
	char input = 0;
	string last_input;
	string name;
	while (input != 10 && input != 13)
	{
		if (Serial.available() > 0)
		{
			input = Serial.read();
			name += input;
		}
	}
	last_input += input;
	debug_message("Got response: ");
	debug_message(name);
	if (name.length() > 1)
	{
		bChanged = true;
		strncpy(settings.wifi_name, name.c_str(), 63);
	}
	
	
	debug_message("\nEnter new network password:\n");
	Serial.flush();
	input = 0;
	last_input.clear();
	string pass;
	while (input != 10 && input != 13)
	{
		if (Serial.available() > 0)
		{
			input = Serial.read();
			pass += input;
		}
	}
	last_input += input;
	debug_message("Got response: ");
	debug_message(pass);
	if (pass.length() > 1)
	{
		bChanged = true;
		strncpy(settings.wifi_pass, pass.c_str(), 31);
	}
	
	Serial.flush();
	delay(1000);
	
	return bChanged;
}

bool do_serial_setup(PheaterStation_Settings& settings)
{
	bool bChanged = false;
	digitalWrite(LED_FLASH, HIGH); 
	
	debug_message("Current network name: ");
	Serial.flush();
	debug_message(settings.wifi_name);
	Serial.flush();
	debug_message("Current network password: ");
	Serial.flush();
	debug_message(settings.wifi_pass);
	
	debug_message("Would you like to change network settings (y/n)\n");
	Serial.flush();
	
	while (1)
	{
		if (Serial.available() > 0)
		{
			char input = Serial.read();
			
			if (input == 'y')
			{
				bChanged = do_serial_network_settings(settings);
				break;
			}
			else if (input == 'n')
			{
				break;
			}
			else
			{
				string msg = "Invalid input: ";
				msg += input;
				debug_message(msg);
				debug_message("Would you like to change network settings (y/n)\n");
				Serial.flush();
			}
		}
	}
	
	return bChanged;
}

void setup() {
	g_chipID = 0;
	
  	pinMode(LED_FLASH, OUTPUT);
	
	pinMode(THERMISTOR, INPUT);
	pinMode(WINDVANE, INPUT);
	
	pinMode(ANEMOMETER, INPUT);
	attachInterrupt(ANEMOMETER, anemometer_interrupt, RISING);
	
	pinMode(RAINGAUGE, INPUT);
	attachInterrupt(RAINGAUGE, raingauge_interrupt, RISING);
	
	pinMode(SETUP, INPUT);
	attachInterrupt(SETUP, setup_interrupt, RISING);
	
	Serial.begin(9600);
	Serial1.begin(9600);
	g_station.set_logger([=](const string& msg) { debug_message(msg); });
	
	wait_for_serial_interrupt();
	
	g_settings = read_eeeprom_settings();
	
	if (g_settings.first_boot)
	{
		g_setup_mode = true;
		
		while(!Serial.isConnected()) // wait for Host to open serial port
			Particle.process();
				
		delay(3000);

		debug_message("Welcome to Pheaterstation");
		Serial.flush();
	}
	else
	{
		connect_to_wifi(g_settings.wifi_name, g_settings.wifi_pass);
		g_found_sensor = g_BMESensor.begin(g_chipID);
	}
}

/*
void i2c_scan()
{
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
}
*/

void do_main_loop()
{
	digitalWrite(LED_FLASH, HIGH);  // Turn ON the LED
	delay(1000);              // Wait for 1000mS = 1 second
	digitalWrite(LED_FLASH, LOW);   // Turn OFF the LED
	delay(1000);              // Wait for 1 second
	
	int digvalue_thermistor = analogRead(THERMISTOR);
	g_station.set_thermistor_voltage( digvalue_thermistor/(float)ADC_MAX * VCC ) ;
	
	//stringstream msg;
	
	//msg << "ambient temperature: " << g_station.get_temperature() << endl;
	
	//msg << "wind speed: " << g_station.get_windspeed() << " avg: " << g_station.get_windspeed_avg()  << endl;
	
	g_BMESensor.readSensor();
	float c_i2c = g_BMESensor.getTemperature_C();
	float pres_mb = g_BMESensor.getPressure_MB();
	float humidity = g_BMESensor.getHumidity();
	
	//msg  << "i2c temp: " << c_i2c << endl;

	//msg << " pressure mb: " << pres_mb << endl;
	
	//msg << " humidity: " << humidity << endl;
	
	int digvalue_wind_vane = analogRead(WINDVANE);
	float vane_voltage = digvalue_wind_vane/(float)ADC_MAX * VCC;
	
	g_station.update_wind_data(micros(), vane_voltage);
	//msg << " vane: " << vane_voltage << endl;
	
	//debug_message(msg.str());	
	
	Particle.process();
}

void do_web_server()
{
	if (restful_client.connected())
	{
		while (restful_client.available())
		{
			String request = restful_client.readString();
			debug_message(request.c_str());
			Serial.flush();
		}
	}
	else
	{
		restful_client = restful_server.available();
	}
}

void loop() {
	if (g_setup_mode)
	{
		debug_message("entering setup mode\n");
		Serial.flush();
		if (do_serial_setup(g_settings))
		{
			g_settings.first_boot = false;
			EEPROM.put(0, g_settings);
		}
		
		System.reset();
	}
	else
	{
		do_main_loop();
		
		do_web_server();
	}
    
}

void anemometer_interrupt()
{
	unsigned long now = micros();
	if (now - g_last_anemometer >= DEBOUNCE_MS * 1000)
	{
		g_last_anemometer = now;
		g_station.set_anemometer_turn(now);
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
	if (now - g_last_rain >= DEBOUNCE_MS * 1000)
	{
		g_last_rain = now;
		debug_message("rain gauage trigger\n");
	}
	else
	{
		debug_message("rain bounce\n");
	}
}

void setup_interrupt()
{
	debug_message("setup trigger\n");
	g_setup_mode = true;
}