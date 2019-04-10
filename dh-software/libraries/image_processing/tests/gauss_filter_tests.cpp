#include "gauss_filter.h"

#include "image_processing.h"
#include "image_converter.h"

#include <gtest/gtest.h>

using namespace dh;
using cv::Mat;

using namespace std;
using namespace testing;

// TODO
/*
TEST( gauss_filter_tests, constructor_wrong_channels_throws_exception )
{
    EXPECT_THROW( dft( 7, 7, 3 ), argument_exception );
}

TEST( gauss_filter_tests, forward_wrong_src_throws_exception )
{
    {
        dft dft( 7, 7, 1 );
        auto magnitudes_32f = Mat( 7, 7, CV_32FC1 );

        {
            auto src = Mat( 7, 8, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 8, 7, CV_32FC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 7, 7, CV_32FC3 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 7, 7, CV_8UC1 );
            EXPECT_THROW( dft.forward( src, magnitudes_32f ), argument_exception );
        }
    }
}
*/
TEST( gauss_filter_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = {  0,   0,   0, 255,   0,   0,   0,
                        0,   0,   0, 255,   0,   0,   0,
                        0,   0,   0, 255,   0,   0,   0,
                      255, 255, 255, 255, 255, 255, 255,
                        0,   0,   0, 255,   0,   0,   0,
                        0,   0,   0, 255,   0,   0,   0,
                        0,   0,   0, 255,   0,   0,   0 };

    auto cols = 7;
    auto rows = 7;

    auto image_8u = Mat( rows, cols, CV_8UC1, static_cast<void*>( data ) );
    auto image_32f = Mat( rows, cols, CV_32FC1 );

    image_converter::convert_8u_32f( image_8u, image_32f );

    gauss_filter gauss( cols, rows, 1, 2 );

    auto filtered_32f = Mat( rows, cols, CV_32FC1 );
    gauss.filter( image_32f, filtered_32f );

    float expected_data[] = { 19.232439f, 30.140631f, 39.931610f, 43.862610f, 39.931610f, 30.140631f, 19.232439f,
                              30.140627f, 45.492401f, 58.862404f, 64.110764f, 58.862404f, 45.492401f, 30.140627f,
                              39.931610f, 58.862404f, 74.981163f, 81.197701f, 74.981163f, 58.862400f, 39.931610f,
                              43.862610f, 64.110764f, 81.197708f, 87.740379f, 81.197708f, 64.110764f, 43.862610f,
                              39.931610f, 58.862404f, 74.981163f, 81.197701f, 74.981163f, 58.862400f, 39.931610f,
                              30.140627f, 45.492405f, 58.862400f, 64.110764f, 58.862400f, 45.492405f, 30.140627f,
                              19.232439f, 30.140629f, 39.931610f, 43.862606f, 39.931610f, 30.140629f, 19.232439f };

    auto expected_filtered_32f = Mat( rows, cols, CV_32FC1, static_cast<void*>( expected_data ) );

    EXPECT_EQ( expected_filtered_32f.rows, filtered_32f.rows );
    EXPECT_EQ( expected_filtered_32f.cols, filtered_32f.cols );
    EXPECT_EQ( expected_filtered_32f.channels(), filtered_32f.channels() );

    auto diff = expected_filtered_32f != filtered_32f;
    bool eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( eq );
}
