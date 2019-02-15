#include "exception_source.h"

#include <sstream>

using namespace std;

namespace dh
{
    exception_source::exception_source( const char* file, const char* function, int line )
    {
        ostringstream s;

        s << "Exception source:" << endl;
        s << "    File: " << file << endl;
        s << "    Function: " << function << endl;
        s << "    Line: " << line << endl;

        _representation = s.str();
    }

    const char* exception_source::c_str() const
    {
        return _representation.c_str();
    }
}
