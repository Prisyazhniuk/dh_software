#pragma once

#include "exception_source.h"

#include <string>

namespace dh
{
    class dh_exception
    {
    public:
        dh_exception( const exception_source& );
        dh_exception( const std::string& message, const exception_source& );
        dh_exception( const dh_exception* inner_exception, const exception_source& );
        dh_exception( const std::string& message, const dh_exception* inner_exception, const exception_source& );
        virtual ~dh_exception();

        const char* c_str() const;

    private:
        std::string _extended_message;

        mutable bool _string_representation_initialized;
        mutable std::string _string_representation;
    };
}
