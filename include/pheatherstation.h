#pragma once
#include <functional>
#include <math.h> 
#include <string> 
#include <deque>

using namespace std;

const float THERMISTOR_B = 4150; //25/50C
const float THERMISTOR_R = 10000;
const float THERM_DIV_R = 9700;
const float K_OFFSET = 273.15;
const float MPH_KPH = 1.60934;

const size_t WIND_SAMPLES = 64;
//https://www.ncdc.noaa.gov/crn/measurements.html
const unsigned long WIND_OLDEST_SAMPLE_MS = 5 * 60 * 1000;

class PheatherStation
{
public:
	float get_temperature() const { return temperature_; }
	void set_temperature(float temp) { temperature_ = temp; }
	float get_windspeed() const { return wind_speed_; }
	float get_windspeed_avg() const { return wind_speed_avg_; }
	
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
	 * @param direction compass direction of wind vane sensor
	 */
	void update_wind_data(unsigned long timestamp, unsigned short direction);
	
private:
	void debugline(const string& msg) const;
	
	// debug/serial output
	function<void(const string&)> logger_ = nullptr;
	
	deque<unsigned long> anemometer_turns_;
	
	// weather (metric)
	float temperature_ = 0;
	float wind_speed_ = 0;
	float wind_speed_avg_ = 0;
	unsigned short  wind_direction_ = 0;
};
