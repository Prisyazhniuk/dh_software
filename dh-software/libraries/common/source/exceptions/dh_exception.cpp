#include "dh_exception.h"
#include "type_info_helpers.h"

#include <typeinfo>
#include <sstream>

using namespace std;

namespace dh
{
    dh_exception::dh_exception( const exception_source& source )
        : dh_exception( "", source )
    {}

    dh_exception::dh_exception( const string& message, const exception_source& source )
        : _string_representation_initialized( false )
    {
        auto class_name = demangle_class_name( typeid( *this ).name() );

        ostringstream s;

        if( !message.empty() )
            s << "Message: " << message << endl;

        s << source.c_str() << endl;

        _extended_message = s.str();
    }

    dh_exception::dh_exception( const dh_exception* inner_exception, const exception_source& source )
        : dh_exception( "", inner_exception, source )
    {}

    dh_exception::dh_exception( const string& message,
                                const dh_exception* inner_exception,
                                const exception_source& source )
        : _string_representation_initialized( false )
    {
        auto class_name = demangle_class_name( typeid( *this ).name() );

        ostringstream s;

        if( !message.empty() )
            s << "Message: " << message << endl;

        s << source.c_str() << endl;
        s << "Inner exception: " << inner_exception->c_str() << endl;

        _extended_message = s.str();
    }

    dh_exception::~dh_exception()
    {}

    const char* dh_exception::c_str() const
    {
        if( !_string_representation_initialized )
        {
            auto class_name = demangle_class_name( typeid( *this ).name() );

            ostringstream s;

            s << "Exception class: " << class_name << endl;
            s << _extended_message << endl;

            _string_representation = s.str();

            _string_representation_initialized = true;
        }

        return _string_representation.c_str();
    }
}
