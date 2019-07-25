#pragma once

#include "image_32fc.h"

#include "exceptions/argument_exception.h"
#include "exceptions/image_processing_exception.h"

#include <ippcore.h>

namespace dh
{
    class dft
    {
    public:
        dft( int width, int height );
        ~dft();

        void forward( const image_32fc& src, image_32fc& dst );

    private:
        const int _flags = IPP_FFT_NODIV_BY_ANY;

        int _width;
        int _height;
        int _channels;

        IppiDFTSpec_C_32fc* _context;
        uint8_t* _init_buffer;
        uint8_t* _work_buffer;
    };
}
