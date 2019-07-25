#include "dft.h"
#include "image_processing.h"
#include "image_converter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <ippi.h>

#include <gtest/gtest.h>

using namespace dh;
using cv::Mat;

using namespace testing;

TEST( dft_tests, forward_wrong_src_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto dst = image_32fc( 7, 7 );

        {
            auto src = image_32fc( 7, 8 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }

        {
            auto src = image_32fc( 8, 7 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, forward_wrong_dst_throws_exception )
{
    {
        dft dft( 7, 7 );
        auto src = image_32fc( 7, 7 );

        {
            auto dst = image_32fc( 7, 8 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }

        {
            auto dst = image_32fc( 8, 7 );
            EXPECT_THROW( dft.forward( src, dst ), argument_exception );
        }
    }
}

TEST( dft_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0 };

    auto width = 7;
    auto height = 7;

    auto src_32fc = image_32fc( width, height );
    auto dst_32fc = image_32fc( width, height );

    auto src_8u = Mat( height, width, CV_8UC1, static_cast<void*>( data ) );

    image_converter::convert_8u_32fc( src_8u, src_32fc );

    dft dft( width, height );

    dft.forward( src_32fc, dst_32fc );

    auto magnitudes_32f = Mat( height, width, CV_32FC1 );

    auto status = ippiMagnitude_32fc32f_C1R( dst_32fc.data(), dst_32fc.step_in_bytes(),
                                             magnitudes_32f.ptr<float>(), int( magnitudes_32f.step ),
                                             { width, height } );
    if( status != ippStsNoErr )
        throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );

    float expected_data[] = { 2295.00000f, 1718.93930f, 424.542940f, 613.482360f, 613.482360f, 424.542940f, 1718.93930f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494170f, 459.494110f, 317.979800f, 1287.47390f,
                               424.54294f,  317.97980f,  78.534523f, 113.485680f, 113.485695f,  78.534523f,  317.97977f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               613.48236f,  459.49411f, 113.485680f, 163.991560f, 163.991560f, 113.485680f,  459.49411f,
                               424.54294f,  317.97977f,  78.534523f, 113.485695f, 113.485680f,  78.534523f,  317.97980f,
                              1718.93930f, 1287.47390f, 317.979800f, 459.494110f, 459.494170f, 317.979800f, 1287.47390f };

    auto expected_magnitudes_32f = Mat( height, width, CV_32FC1, static_cast<void*>( expected_data ) );

    auto diff = expected_magnitudes_32f != magnitudes_32f;
    bool magnitudes_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( magnitudes_are_eq );
}
