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

    image_32fc::image_32fc( int width, int height, const Ipp32fc* data, int data_step_in_bytes )
        : image_32fc( width, height )
    {
        auto dst = reinterpret_cast<uint8_t*>( _data );
        auto src = reinterpret_cast<const uint8_t*>( data );
        auto size = size_t(width) * sizeof(Ipp32fc);

        for( int row = 0; row < height; row++ )
            memcpy( dst+row*step_in_bytes(), src+row*data_step_in_bytes, size );
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

    int image_32fc::step_in_elements() const
    {
        return _step_in_bytes / int( sizeof( Ipp32fc ) );
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
