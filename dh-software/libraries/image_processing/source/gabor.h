#pragma once

#include "image_32fc.h"

#include "exceptions/image_processing_exception.h"

namespace dh
{
    class gabor
    {
    public:
        static void make_kernel( image_32fc& dst, float lambda, float distance, float pixel_size );
    };
}
