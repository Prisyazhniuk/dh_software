#include "label_marker.h"
#include "dh_string.h"

#include <ippcore.h>
#include <ippcv.h>
#include <ipps.h>

using namespace cv;

namespace dh
{
    label_marker::label_marker( int width, int height, int channels )
    {
        if( channels != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        // TODO
        // check image data type

        int buffer_size;
        auto status = ippiLabelMarkersGetBufferSize_8u_C1R( { width, height }, &buffer_size );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiLabelMarkersGetBufferSize_8u_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        _buffer = ippsMalloc_8u( buffer_size );
        if( !_buffer )
        {
            auto msg = dh_string::fs( "ippMalloc for _buffer failed (%d bytes)", buffer_size );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }

    label_marker::~label_marker()
    {
        if( _buffer )
        {
            ippsFree( _buffer );
            _buffer = nullptr;
        }
    }

    size_t label_marker::mark( const cv::Mat& m )
    {
        // TODO
        // some checks like fft

        int count = 0;
        auto roi = IppiSize{ m.cols, m.rows };
        auto status = ippiLabelMarkers_8u_C1IR( m.data, int( m.step ), roi,
                                                1, 254, ippiNormL1, &count, _buffer );

        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );

        return size_t( count );
    }
}
