#include "type_info_helpers.h"

#include <cxxabi.h>
#include <climits>
#include <memory>

using namespace std;

namespace dh
{
    string demangle_class_name( const char* mangled )
    {
        int status = INT_MIN;
        auto demangled = abi::__cxa_demangle( mangled, nullptr, nullptr, &status );

        string result( status == 0 ? demangled : "" );

        free( demangled );

        return result;
    }
}
