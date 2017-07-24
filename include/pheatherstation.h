#pragma once
#include <functional>
#include <math.h> 
#include <string> 

using namespace std;

const double THERMISTOR_B = 4150; //25/50C
const double THERMISTOR_R = 10000;
const double THERM_DIV_R = 9700;
const double K_OFFSET = 273.15;

class PheatherStation
{
public:
	PheatherStation(double vcc) : vcc_(vcc) {}

	double get_temperature() const { return temperature_; }
	
	/**
	 * @brief Set the external temperature thermistor voltage (A2 on photon)
	 * @param voltage
	 */
	void set_thermistor_voltage(double voltage);
	
	/**
	 * @brief serial / USB debug string output
	 */
	void set_logger(function<void(const string&)> logfunc) { logger_ = logfunc; }
	
private:
	void debugline(const string& msg) const;
	
	// hardware stuff
	double vcc_ = 0;
	function<void(const string&)> logger_ = nullptr;
	
	// weather
	double temperature_ = 0;
	
};
