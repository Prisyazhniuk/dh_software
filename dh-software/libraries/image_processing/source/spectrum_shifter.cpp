#include "spectrum_shifter.h"
#include "dh_string.h"

#include <ippi.h>
#include <ippcore.h>

using namespace std;

namespace dh
{
    spectrum_shifter::spectrum_shifter( int width, int height )
        : _buffer( width, height % 2 ? height/2 + 1 : height/2 )
    {}

    void spectrum_shifter::shift( image_32fc& image )
    {
        auto step = image.step_in_bytes();
        auto buffer_step = _buffer.step_in_bytes();

        auto width = image.width();
        auto height = image.height();

        auto height_2 = height % 2 ? height/2 + 1 : height/2;
        auto width_2 = width % 2 ? width/2 + 1 : width/2;


        // 1 and 2 to buffer
        auto status = ippiCopy_32f_C1R( reinterpret_cast<Ipp32f*>( image.data() ),
                                        step,
                                        reinterpret_cast<Ipp32f*>( _buffer.data() ),
                                        buffer_step,
                                        { 2*width, height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_32f_C1R (1 and 2 quadrants) failed: %s",
                                      ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // 4 to 2
        status = ippiCopy_32f_C1R( reinterpret_cast<Ipp32f*>( &image.at( 0, height_2 ) ),
                                   step,
                                   reinterpret_cast<Ipp32f*>( &image.at( width/2, 0 ) ),
                                   step,
                                   { 2*width_2, height/2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_32f_C1R (4 quadrant) failed: %s",
                                      ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // 3 to 1
        status = ippiCopy_32f_C1R( reinterpret_cast<Ipp32f*>( &image.at( width_2, height_2 ) ),
                                   step,
                                   reinterpret_cast<Ipp32f*>( &image.at( 0, 0 ) ),
                                   step,
                                   { 2*(width/2), height/2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_32f_C1R (3 quadrant) failed: %s",
                                      ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // buffered 1 to 3
        status = ippiCopy_32f_C1R( reinterpret_cast<Ipp32f*>( _buffer.data() ),
                                   buffer_step,
                                   reinterpret_cast<Ipp32f*>( &image.at( width/2, height/2 ) ),
                                   step,
                                   { 2*(width_2), height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_32f_C1R (1 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // buffered 2 to 4
        status = ippiCopy_32f_C1R( reinterpret_cast<Ipp32f*>( &_buffer.at( width_2, 0 ) ),
                                   buffer_step,
                                   reinterpret_cast<Ipp32f*>( &image.at( 0, height/2 ) ),
                                   step,
                                   { 2*(width/2), height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_32f_C1R (2 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }
}
