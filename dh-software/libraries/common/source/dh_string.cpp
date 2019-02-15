#include "dh_string.h"

#include <cstdarg>
#include <memory>

using namespace std;

namespace dh
{
    string dh_string::fs( const char* format, ... )
    {
        va_list args;
        va_start( args, format );

        auto length = size_t( vsnprintf( nullptr, 0, format, args ) );
        unique_ptr<char[]> buffer( new char[ length + 1 ] );

        vsnprintf( buffer.get(), length + 1, format, args );

        va_end( args );

        return string( buffer.get(), buffer.get() + length );
    }
}
