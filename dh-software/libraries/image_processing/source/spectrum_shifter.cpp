#include "spectrum_shifter.h"
#include "dh_string.h"

#include <ippi.h>
#include <ippcore.h>

using namespace cv;
using namespace std;

namespace dh
{
    spectrum_shifter::spectrum_shifter( int step, int height, int channels )
    {
        if( step <= 0 )
            throw argument_exception( "step is invalid", get_exception_source() );

        if( height <= 0 )
            throw argument_exception( "height is invalid", get_exception_source() );

        if( channels != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        auto height_2 = height % 2 ? height/2 + 1 : height/2;
        auto buffer_size = step * height_2 * channels;
        _buffer = make_unique<uint8_t[]>( size_t( buffer_size ) );
    }

    void spectrum_shifter::shift( Mat& m )
    {
        int step = int( m.step );

        int width = m.cols;
        int height = m.rows;

        auto height_2 = height % 2 ? height/2 + 1 : height/2;
        auto width_2 = width % 2 ? width/2 + 1 : width/2;

        // 1 and 2 to buffer
        auto status = ippiCopy_8u_C1R( m.data, step,
                                       _buffer.get(), step,
                                       { width, height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_8u_C1R (1 and 2 quadrants) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // 4 to 2
        status = ippiCopy_8u_C1R( m.ptr( height_2, 0 ), step,
                                  m.ptr( 0, width/2 ), step,
                                  { width_2, height/2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_8u_C1R (4 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // 3 to 1
        status = ippiCopy_8u_C1R( m.ptr( height_2, width_2 ), step,
                                  m.ptr( 0, 0 ), step,
                                  { width/2, height/2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_8u_C1R (3 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // buffered 1 to 3
        status = ippiCopy_8u_C1R( _buffer.get(), step,
                                  m.ptr( height/2, width/2 ), step,
                                  { width_2, height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_8u_C1R (1 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        // buffered 2 to 4
        status = ippiCopy_8u_C1R( _buffer.get() + width_2, step,
                                  m.ptr( height/2, 0 ), step,
                                  { width/2, height_2 } );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiCopy_8u_C1R (2 quadrant) failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }
}
