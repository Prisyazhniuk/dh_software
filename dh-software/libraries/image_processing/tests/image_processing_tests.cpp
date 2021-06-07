#include "image_processing.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace cv;
using namespace testing;

TEST( image_processing_tests, max_8u_unsupported_channels_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_8u = Mat( 3, 3, CV_8UC3 );
    EXPECT_THROW( image_processing::max_8u( image_8u ), argument_exception );
}

TEST( image_processing_tests, max_8u_unsupported_depth_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_32f = Mat( 3, 3, CV_32FC1 );
    EXPECT_THROW( image_processing::max_8u( image_32f ), argument_exception );
}

TEST( image_processing_tests, max_8u_works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 10, 20, 30,
                       40, 50, 60,
                       70, 80, 90 };

    auto width = 3;
    auto height = 3;

    auto image_8u = Mat( height, width, CV_8UC1, static_cast<void*>( data ) );

    EXPECT_EQ( image_processing::max_8u( image_8u ), 90 );
}


TEST( image_processing_tests, min_max_8u_unsupported_channels_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_8u = Mat( 3, 3, CV_8UC3 );
    uint8_t min, max;

    EXPECT_THROW( image_processing::min_max_8u( image_8u, min, max ), argument_exception );
}

TEST( image_processing_tests, min_max_8u_unsupported_depth_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_32f = Mat( 3, 3, CV_32FC1 );
    uint8_t min, max;

    EXPECT_THROW( image_processing::min_max_8u( image_32f, min, max ), argument_exception );
}

TEST( image_processing_tests, min_max_8u_works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 10, 20, 30,
                       40, 50, 60,
                       70, 80, 90 };

    auto width = 3;
    auto height = 3;

    auto image_8u = Mat( height, width, CV_8UC1, static_cast<void*>( data ) );

    uint8_t min, max;
    image_processing::min_max_8u( image_8u, min, max );

    EXPECT_EQ( min, 10 );
    EXPECT_EQ( max, 90 );
}


TEST( image_processing_tests, mean_8u_unsupported_channels_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_8u = Mat( 3, 3, CV_8UC3 );
    EXPECT_THROW( image_processing::mean_8u( image_8u ), argument_exception );
}

TEST( image_processing_tests, mean_8u_unsupported_depth_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_32f = Mat( 3, 3, CV_32FC1 );
    EXPECT_THROW( image_processing::mean_8u( image_32f ), argument_exception );
}

TEST( image_processing_tests, mean_8u_works )
{
    image_processing::initialize_ipp();

    uint8_t data[] = { 11, 11, 22,
                       22, 33, 33,
                       44, 44, 55 };

    auto width = 3;
    auto height = 3;

    auto image_8u = Mat( height, width, CV_8UC1, static_cast<void*>( data ) );

    EXPECT_EQ( image_processing::mean_8u( image_8u ), 31 );
}


TEST( image_processing_tests, min_max_32f_unsupported_channels_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_32f = Mat( 3, 3, CV_32FC3 );
    float min, max;

    EXPECT_THROW( image_processing::min_max_32f( image_32f, min, max ), argument_exception );
}

TEST( image_processing_tests, min_max_32f_unsupported_depth_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_8u = Mat( 3, 3, CV_8UC1 );
    float min, max;

    EXPECT_THROW( image_processing::min_max_32f( image_8u, min, max ), argument_exception );
}

TEST( image_processing_tests, min_max_32f_works )
{
    image_processing::initialize_ipp();

    float data[] = { 10.0f, 20.0f, 30.0f,
                     40.0f, 50.0f, 60.0f,
                     70.0f, 80.0f, 90.0f };

    auto width = 3;
    auto height = 3;

    auto image_32f = Mat( height, width, CV_32FC1, static_cast<void*>( data ) );

    float min, max;
    image_processing::min_max_32f( image_32f, min, max );

    EXPECT_EQ( min, 10.0f );
    EXPECT_EQ( max, 90.0f );
}


TEST( image_processing_tests, mean_32f_unsupported_channels_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_32f = Mat( 3, 3, CV_32FC3 );
    EXPECT_THROW( image_processing::mean_32f( image_32f ), argument_exception );
}

TEST( image_processing_tests, mean_32f_unsupported_depth_throws_exception )
{
    image_processing::initialize_ipp();

    auto image_8u = Mat( 3, 3, CV_8UC1 );
    EXPECT_THROW( image_processing::mean_32f( image_8u ), argument_exception );
}

TEST( image_processing_tests, mean_32f_works )
{
    image_processing::initialize_ipp();

    float data[] = { 11, 11, 22,
                     22, 33, 33,
                     44, 44, 55 };

    auto width = 3;
    auto height = 3;

    auto image_32f = Mat( height, width, CV_32FC1, static_cast<void*>( data ) );

    EXPECT_FLOAT_EQ( image_processing::mean_32f( image_32f ), 30.555555f );
}
