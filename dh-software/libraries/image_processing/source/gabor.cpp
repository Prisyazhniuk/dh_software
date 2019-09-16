#include "gabor.h"

#include <math.h>

#include <ipps.h>

namespace dh
{
    void gabor::make_kernel( image_32fc& dst, float lambda, float distance, float pixel_size )
    {
        const auto d = double( distance );
        const auto dx = double( pixel_size );
        const auto lmbd = double( lambda );

        const auto cols = dst.width();
        const auto rows = dst.height();

        const auto half_cols = cols / 2;
        const auto half_rows = rows / 2;

        for( auto r = 0; r < rows; r++ )
            for( auto c = 0; c < cols; c++ )
            {
                auto dr = r + 1 - half_rows;
                auto dc = c + 1 - half_cols;

                auto a = ( 2*IPP_PI*d/lmbd ) + ( IPP_PI*dx*dx*( dr*dr + dc*dc ) / (d*lmbd) );

                auto& k = dst.at( c, r );
                k.re = float( cos( a ) );
                k.im = float( sin( a ) );
            }

        auto c = Ipp32fc{ 0, -1 / ( distance * lambda ) };
        auto status = ippsMulC_32fc_I( c, dst.data(), dst.height() * dst.step_in_elements() );

        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }
}
