#include "binarizer.h"
#include "dh_string.h"

#include <ippcore.h>
#include <ippi.h>

using namespace cv;

namespace dh
{
    void binarizer::binarize( const cv::Mat& src, cv::Mat& dst, float threshold )
    {
        if( src.channels() != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        // TODO
        // some more checks

        auto roi = IppiSize{ src.cols, src.rows };

        uint8_t max = 0;
        ippiMax_8u_C1R( src.data, static_cast<int>( src.step ), roi, &max );

        auto threshold_int = uint8_t( max * threshold );

        auto status = ippiThreshold_Val_8u_C1R( src.data, static_cast<int>( src.step ),
                                                dst.data, static_cast<int>( dst.step ),
                                                roi, threshold_int, 0, ippCmpLess );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiThreshold_Val_8u_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }
}
