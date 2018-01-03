
#include "pheatherstation.h"
#include <math.h> 
#include <string>
#include <cstdlib>
#include <stdio.h>


void PheatherStation::debugline(const string& msg) const
{
		if (logger_)
		{
			logger_(msg + "\n");
		}
}

//Davis instrument formula  V = P(2.25/T) , V = speed in mph, P = no. of pulses per sample period, T = sample period in seconds
void PheatherStation::set_anemometer_turn(unsigned long timestamp_microsec)
{
	anemometer_turns_.push_back(timestamp_microsec);
	update_wind_data(timestamp_microsec, wind_direction_);
}

void PheatherStation::update_wind_data(unsigned long timestamp_microsec, unsigned short direction)
{
	wind_speed_ = 0;
	wind_speed_avg_ = 0;
	wind_direction_ = direction;
	
	if (!anemometer_turns_.empty())
	{
		// prune the wind speed data
		while (anemometer_turns_.size() > WIND_SAMPLES ||
			timestamp_microsec - anemometer_turns_.front() >= WIND_OLDEST_SAMPLE_MS * 1000)
		{
			anemometer_turns_.pop_front();
			if (anemometer_turns_.empty()) break;
		}

		// calculate wind speed/averages
		double duration_sec = (timestamp_microsec - anemometer_turns_.front()) / 1000000;
		if (duration_sec > 0)
		{
			// 5 minute avg https://www.ncdc.noaa.gov/crn/measurements.html
			float v_mph_avg = anemometer_turns_.size() * 2.25 / duration_sec;
			wind_speed_avg_= v_mph_avg * MPH_KPH;
			
			// instantaneous (2 sec)
			size_t pulse_cnt = 0;
			duration_sec = 0;
			auto iter = anemometer_turns_.rbegin();
			if ((timestamp_microsec - *iter) / 1000000 < 2 )
			{
				while (iter !=anemometer_turns_.rend() && duration_sec < 2)
				{
					duration_sec = (timestamp_microsec - *iter) / 1000000;
					pulse_cnt++;
					iter++;
				}
				
				if (duration_sec > 0)
				{
					float v_mph = pulse_cnt * 2.25 / duration_sec; 
					wind_speed_ = v_mph * MPH_KPH;
				}
			}
		}
	}
	
}
