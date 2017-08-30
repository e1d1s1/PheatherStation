#pragma once
#include <functional>
#include <math.h> 
#include <string> 
#include <list>

using namespace std;

const double THERMISTOR_B = 4150; //25/50C
const double THERMISTOR_R = 10000;
const double THERM_DIV_R = 9700;
const double K_OFFSET = 273.15;
const double MPH_KPH = 1.60934;

const size_t WIND_SAMPLES = 64;
//https://www.ncdc.noaa.gov/crn/measurements.html
const unsigned long WIND_OLDEST_SAMPLE_MS = 5 * 60 * 1000;

class PheatherStation
{
public:
	PheatherStation(double vcc) : vcc_(vcc) {}

	double get_temperature() const { return temperature_; }
	double get_windspeed() const { return wind_speed_; }
	double get_windspeed_avg() const { return wind_speed_avg_; }
	
	/**
	 * @brief Set the external temperature thermistor voltage (A2 on photon)
	 * @param voltage
	 */
	void set_thermistor_voltage(double voltage);
	
	/**
	 * @brief serial / USB debug string output
	 */
	void set_logger(function<void(const string&)> logfunc) { logger_ = logfunc; }
	
	/**
	 * @brief set the timestamp of an anemometer turn. 
	 * Davis instrument formula  V = P(2.25/T) , V = speed in mph, P = no. of pulses per sample period, T = sample period in seconds
	 * @param timestamp
	 */
	void set_anemometer_turn(unsigned long timestamp);
	
	/**
	 * @brief recalcualte avg and instant wind readings
	 * @param timestamp
	 */
	void update_wind_data(unsigned long timestamp);
	
private:
	void debugline(const string& msg) const;
	
	// hardware stuff
	double vcc_ = 0;
	function<void(const string&)> logger_ = nullptr;
	
	list<unsigned long> anemometer_turns_;
	
	// weather (metric)
	double temperature_ = 0;
	double wind_speed_ = 0;
	double wind_speed_avg_ = 0;
};
