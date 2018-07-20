#pragma once

#include <vector>
#include <string>

class WebHandler
{
public:
	std::string get_response(const std::vector<std::string>& headers) const;
	
private:

};