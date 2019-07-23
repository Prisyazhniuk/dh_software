#pragma once

#include "exceptions/argument_exception.h"

#include <ippcore.h>

namespace dh
{
    class image_32fc
    {
    public:
        image_32fc( int width, int height );
        ~image_32fc();

        int width() const;
        int height() const;
        int step_in_bytes() const;

        Ipp32fc& at( int x, int y ) const;

        Ipp32fc* data() const;

    private:
        int _width;
        int _height;
        int _step_in_bytes;

        Ipp32fc* _data;
    };
}
