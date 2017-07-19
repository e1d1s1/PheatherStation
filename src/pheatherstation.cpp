
#include "pheatherstation.h"


void PheatherStation::set_thermistor_voltage(double voltage)
{
	
}

void PheatherStation::log(const string& msg) const
{
		if (logger_)
		{
			logger_(msg);
		}
}
