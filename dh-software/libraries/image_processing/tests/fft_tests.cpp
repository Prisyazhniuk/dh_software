#include "fft.h"
#include "image_processing.h"
#include "image_converter.h"

#include <opencv2/highgui.hpp>
#include <opencv2/core.hpp>

#include <iostream>

#include <gtest/gtest.h>

using namespace cv;
using namespace std;
using namespace testing;

TEST( fft_tests, constructor_wrong_channels_throws_exception )
{
    EXPECT_THROW( fft( 7, 7, 3 ), argument_exception );
}

TEST( fft_tests, forward_wrong_src_throws_exception )
{
    {
        fft fft( 7, 7, 1 );

        auto cols = fft.get_width();
        auto rows = fft.get_height();
        auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );

        {
            auto src = Mat( 7, 8, CV_32FC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 8, 7, CV_32FC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 8, 8, CV_32FC3 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto src = Mat( 8, 8, CV_8UC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }
    }
}

TEST( fft_tests, forward_wrong_magnitudes_throws_exception )
{
    {
        fft fft( 7, 7, 1 );

        auto cols = fft.get_width();
        auto rows = fft.get_height();
        auto src = Mat( rows, cols, CV_32FC1 );

        {
            auto magnitudes_32f = Mat( 7, 8, CV_32FC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 8, 7, CV_32FC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 8, 8, CV_32FC3 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }

        {
            auto magnitudes_32f = Mat( 8, 8, CV_8UC1 );
            EXPECT_THROW( fft.forward( src, magnitudes_32f ), argument_exception );
        }
    }
}

TEST( fft_tests, works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0, 255, 255, 255, 0, 0,
                       0, 0,   0,   0,   0, 0, 0,
                       0, 0,   0,   0,   0, 0, 0 };

    auto image_8u = Mat( 7, 7, CV_8UC1, static_cast<void*>( data ) );

    IppiSize roi = { image_8u.cols, image_8u.rows };

    fft fft( image_8u.cols, image_8u.rows, image_8u.channels() );

    auto cols = fft.get_width();
    auto rows = fft.get_height();

    auto image_32f = Mat( rows, cols, CV_32FC1, Scalar( 0 ) );
    auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );

    image_converter::convert_8u_32f( image_8u, image_32f, roi );

    fft.forward( image_32f, magnitudes_32f );

    float expected_data[] = { 2295.00000f, 1846.87330f, 765.00000f, 316.87338f, 765.00000f, 316.87338f, 765.00000f, 1846.87330f,
                              1846.87340f, 1486.24890f, 615.62445f, 255.00000f, 615.62445f, 255.00000f, 615.62445f, 1486.24890f,
                               765.00000f,  615.62445f, 255.00000f, 105.62446f, 255.00000f, 105.62446f, 255.00000f,  615.62445f,
                               316.87332f,  255.00000f, 105.62446f, 43.751091f, 105.62446f, 43.751091f, 105.62446f,  255.00000f,
                               765.00000f,  615.62445f, 255.00000f, 105.62446f, 255.00000f, 105.62446f, 255.00000f,  615.62445f,
                               316.87332f,  255.00000f, 105.62446f, 43.751091f, 105.62446f, 43.751091f, 105.62446f,  255.00000f,
                               765.00000f,  615.62445f, 255.00000f, 105.62446f, 255.00000f, 105.62446f, 255.00000f,  615.62445f,
                              1846.87340f, 1486.24890f, 615.62445f, 255.00000f, 615.62445f, 255.00000f, 615.62445f, 1486.24890f };

    auto expected_magnitudes_32f = Mat( 8, 8, CV_32FC1, static_cast<void*>( expected_data ) );

    EXPECT_EQ( expected_magnitudes_32f.rows, magnitudes_32f.rows );
    EXPECT_EQ( expected_magnitudes_32f.cols, magnitudes_32f.cols );
    EXPECT_EQ( expected_magnitudes_32f.channels(), magnitudes_32f.channels() );

    auto diff = expected_magnitudes_32f != magnitudes_32f;
    bool magnitudes_are_eq = cv::countNonZero(diff) == 0;

    EXPECT_TRUE( magnitudes_are_eq );
}
