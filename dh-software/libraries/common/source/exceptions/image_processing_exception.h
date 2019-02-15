#pragma once

#include "exceptions/dh_exception.h"

namespace dh
{
    class image_processing_exception : public dh_exception
    {
    public:
        image_processing_exception( const exception_source& source )
            : dh_exception( source )
        {}

        image_processing_exception( const std::string& message, const exception_source& source )
            : dh_exception( message, source )
        {}

        image_processing_exception( const dh_exception* inner_exception, const exception_source& source )
            : dh_exception( inner_exception, source )
        {}
    };
}
