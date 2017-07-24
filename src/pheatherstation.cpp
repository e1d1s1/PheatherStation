
#include "pheatherstation.h"
#include <math.h> 
#include <string>
#include <stdio.h>


void PheatherStation::set_thermistor_voltage(double voltage)
{
	// thermisior is on the "upper" leg, connected to vcc
	double r_therm = (vcc_/voltage - 1) * THERM_DIV_R;
	
	//https://en.wikipedia.org/wiki/Thermistor
	temperature_ = 1 / ( 1/(25 + K_OFFSET) + 1 / THERMISTOR_B * log(r_therm/THERMISTOR_R)) - K_OFFSET;
}

void PheatherStation::debugline(const string& msg) const
{
		if (logger_)
		{
			logger_(msg + "\n");
		}
}
