#pragma once

struct PheaterStation_Settings
{
	uint8_t first_boot = 0;
	char wifi_pass[64] = {0};
	char wifi_name[32] = {0};
};