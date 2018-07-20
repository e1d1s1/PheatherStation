#include "web_handler.h"
#include <iostream>
#include <sstream>
#include <iterator>

using namespace std;

std::string WebHandler::get_response(const std::vector<std::string>& headers) const
{
	unsigned int code = 400;
	string reply_content;
	if (!headers.empty())
	{
		if (headers[0].length() > 3 && headers[0].compare(0, 3, "GET") == 0)
		{
			reply_content = "getting: " + headers[0].substr(3);
			code = 200;
			
			std::istringstream iss(headers[0]);
			std::vector<std::string> request_line_items(std::istream_iterator<std::string>{iss},
				 std::istream_iterator<std::string>());

			if (request_line_items.size() == 3)
			{
				if (request_line_items[1] == "/networkname")
				{
					reply_content = "WIFI GOES HERE";
				}
			}
		}
	}
	
	stringstream reply_stream;
	reply_stream << "HTTP/1.0 ";
	reply_stream << code;
	reply_stream << "\r\n";
	reply_stream << "Content-Length: " << reply_content.length() << "\r\n\r\n";
			
	return reply_stream.str() + reply_content;
}