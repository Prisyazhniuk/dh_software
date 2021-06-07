#pragma once

#include "image_32fc.h"

#include "exceptions/image_processing_exception.h"

namespace dh
{
    class spectrum_shifter
    {
    public:
        spectrum_shifter( int width, int height );

        void shift( image_32fc& );

    private:
        image_32fc _buffer;
    };
}
