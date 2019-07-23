#include "image_32fc.h"

#include <ippi.h>

namespace dh
{
    image_32fc::image_32fc( int width, int height )
        : _width( width )
        , _height( height )
        , _data( nullptr )
    {
        if( width <= 0 )
            throw argument_exception( "width not positive", get_exception_source() );

        if( height <= 0 )
            throw argument_exception( "height not positive", get_exception_source() );

        _data = ippiMalloc_32fc_C1( width, height, &_step_in_bytes );
    }

    image_32fc::~image_32fc()
    {
        ippiFree( _data );
    }

    int image_32fc::width() const
    {
        return _width;
    }

    int image_32fc::height() const
    {
        return _height;
    }

    int image_32fc::step_in_bytes() const
    {
        return _step_in_bytes;
    }

    Ipp32fc& image_32fc::at( int x, int y ) const
    {
        auto offset = size_t(_step_in_bytes*y) + sizeof(Ipp32fc)*size_t(x);
        auto ptr = reinterpret_cast<uint8_t*>( _data );
        return *reinterpret_cast<Ipp32fc*>( ptr + offset );
    }

    Ipp32fc* image_32fc::data() const
    {
        return _data;
    }
}
