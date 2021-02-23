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
        auto length = size_t( vsnprintf( nullptr, 0, format, args ) ) + 1;
        va_end( args );

        auto buffer = make_unique<char[]>( length );

        va_start( args, format );
        vsnprintf( buffer.get(), length, format, args );
        va_end( args );

        return string( buffer.get(), length - 1 );
    }
}
