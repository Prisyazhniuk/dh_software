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

        int width();
        int height();
        int step();

        Ipp32fc* data();

    private:
        int _width;
        int _height;
        int _step;

        Ipp32fc* _data;
    };
}
