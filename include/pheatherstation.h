#pragma once
#include <functional>

using namespace std;

class PheatherStation
{
public:
	double get_temperature() const;
	
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
	void log(const string& msg) const;
	
	function<void(const string&)> logger_ = nullptr;
};
