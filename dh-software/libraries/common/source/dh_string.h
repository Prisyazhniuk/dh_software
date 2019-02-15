#pragma once

#include <string>

using std::string;

namespace dh
{
    class dh_string
    {
    public:
        static string fs( const char* format, ... );
    };
}
