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

        _data = ippiMalloc_32fc_C1( width, height, &_step );
    }

    image_32fc::~image_32fc()
    {
        ippiFree( _data );
    }

    int image_32fc::width()
    {
        return _width;
    }

    int image_32fc::height()
    {
        return _height;
    }

    int image_32fc::step()
    {
        return _step;
    }

    Ipp32fc* image_32fc::data()
    {
        return _data;
    }
}
