#pragma once

#include "exceptions/dh_exception.h"

namespace dh
{
    class argument_exception : public dh_exception
    {
    public:
        argument_exception( const exception_source& source )
            : dh_exception( source )
        {}

        argument_exception( const std::string& message, const exception_source& source )
            : dh_exception( message, source )
        {}

        argument_exception( const dh_exception* inner_exception, const exception_source& source )
            : dh_exception( inner_exception, source )
        {}
    };
}
