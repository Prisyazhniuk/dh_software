#include "image_processing.h"

#include <limits>
#include <ippcore.h>
#include <ippi.h>

using namespace std;
using namespace cv;

namespace dh
{
    void image_processing::initialize_ipp()
    {
        auto status = ippInit();

        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }

    uint8_t image_processing::max_8u( const Mat& image_8u )
    {
        if( image_8u.channels() != 1 )
            throw argument_exception( "image_8u has wrong channels count", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        uint8_t max;

        auto status = ippiMax_8u_C1R( image_8u.data,
                                      int( image_8u.step ),
                                      { image_8u.cols, image_8u.rows },
                                      &max );
        if( status != ippStsNoErr )
        {
            auto message = dh_string::fs( "ippiMax_8u_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( message, get_exception_source() );
        }

        return max;
    }

    void image_processing::min_max_8u( const Mat& image_8u, uint8_t& min, uint8_t& max )
    {
        if( image_8u.channels() != 1 )
            throw argument_exception( "image_8u has wrong channels count", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        auto status = ippiMinMax_8u_C1R( image_8u.data,
                                         int( image_8u.step ),
                                         { image_8u.cols, image_8u.rows },
                                         &min, &max );
        if( status != ippStsNoErr )
        {
            auto message = dh_string::fs( "ippiMinMax_8u_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( message, get_exception_source() );
        }
    }

    uint8_t image_processing::mean_8u( const Mat& image_8u )
    {
        if( image_8u.channels() != 1 )
            throw argument_exception( "image_8u has wrong channels count", get_exception_source() );

        if( image_8u.depth() != CV_8U )
            throw argument_exception( "image_8u has wrong depth", get_exception_source() );

        Ipp64f mean;

        auto status = ippiMean_8u_C1R( image_8u.data,
                                      int( image_8u.step ),
                                      { image_8u.cols, image_8u.rows },
                                      &mean );
        if( status != ippStsNoErr )
        {
            auto message = dh_string::fs( "ippiMean_8u_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( message, get_exception_source() );
        }

        if( mean < double(numeric_limits<uint8_t>::lowest()) ||
            mean > double(numeric_limits<uint8_t>::max()) )
        {
            auto message = dh_string::fs( "mean value out of range %f", mean );
            throw image_processing_exception( message, get_exception_source() );
        }

        return uint8_t(round( mean ));
    }

    void image_processing::min_max_32f( const Mat& image_32f, float& min, float& max )
    {
        if( image_32f.channels() != 1 )
            throw argument_exception( "image_32f has wrong channels count", get_exception_source() );

        if( image_32f.depth() != CV_32F )
            throw argument_exception( "image_32f has wrong depth", get_exception_source() );

        auto status = ippiMinMax_32f_C1R( image_32f.ptr<float>(),
                                          int( image_32f.step ),
                                          { image_32f.cols, image_32f.rows },
                                          &min,
                                          &max );
        if( status != ippStsNoErr )
        {
            auto message = dh_string::fs( "ippiMinMax_32f_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( message, get_exception_source() );
        }
    }

    float image_processing::mean_32f( const Mat& image_32f )
    {
        if( image_32f.channels() != 1 )
            throw argument_exception( "image_32f has wrong channels count", get_exception_source() );

        if( image_32f.depth() != CV_32F )
            throw argument_exception( "image_32f has wrong depth", get_exception_source() );

        Ipp64f mean;

        auto status = ippiMean_32f_C1R( image_32f.ptr<float>(),
                                        int( image_32f.step ),
                                        { image_32f.cols, image_32f.rows },
                                        &mean,
                                        IppHintAlgorithm::ippAlgHintAccurate );
        if( status != ippStsNoErr )
        {
            auto message = dh_string::fs( "ippiMean_32f_C1R failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( message, get_exception_source() );
        }

        if( mean < double(numeric_limits<float>::lowest()) ||
            mean > double(numeric_limits<float>::max()) )
        {
            auto message = dh_string::fs( "mean value out of range %f", mean );
            throw image_processing_exception( message, get_exception_source() );
        }

        return float(mean);
    }
}
