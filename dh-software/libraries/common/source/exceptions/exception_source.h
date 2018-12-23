#pragma once

#include <string>

class exception_source
{
public:
	exception_source( const char* file, const char* function, int line );

	const char* c_str() const;

private:
	std::string _representation;
};

#define get_exception_source() exception_source( __FILE__, __PRETTY_FUNCTION__, __LINE__ )
