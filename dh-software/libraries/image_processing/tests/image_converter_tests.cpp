#include "image_converter.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace cv;
using namespace testing;

TEST( image_converter_tests, convert_8u_32f_different_size_throws_exception )
{
	{
        auto image_8u = Mat( 3, 3, CV_8UC1 );
        auto image_32f = Mat( 4, 4, CV_32FC1 );

        EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f, { 4, 3 } ), argument_exception );
        EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f, { 3, 4 } ), argument_exception );
	}

    {
        auto image_8u = Mat( 4, 4, CV_8UC1 );
        auto image_32f = Mat( 3, 3, CV_32FC1 );

        EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f, { 4, 3 } ), argument_exception );
        EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f, { 3, 4 } ), argument_exception );
    }
}

TEST( image_converter_tests, convert_8u_32f_different_channels_throws_exception )
{
    auto image_8u = Mat( 3, 3, CV_8UC1 );
    auto image_32f = Mat( 3, 3, CV_32FC3 );

    EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
}

TEST( image_converter_tests, convert_8u_32f_wrong_format_throws_exception )
{
	{
		auto image_8u = Mat( 3, 3, CV_32FC1 );
		auto image_32f = Mat( 3, 3, CV_32FC1 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}

	{
		auto image_8u = Mat( 3, 3, CV_8UC1 );
		auto image_32f = Mat( 3, 3, CV_8UC1 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}
}

TEST( image_converter_tests, convert_8u_32f_unsupported_channels_throws_exception )
{
	{
		auto image_8u = Mat( 3, 3, CV_8UC2 );
		auto image_32f = Mat( 3, 3, CV_32FC2 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}

	{
		auto image_8u = Mat( 3, 3, CV_8UC4 );
		auto image_32f = Mat( 3, 3, CV_32FC4 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}
}

TEST( image_converter_tests, convert_8u_32f_c1_works )
{
	auto image_8u = Mat( 3, 3, CV_8UC1 );
	auto image_32f = Mat( 3, 3, CV_32FC1 );

	uint8_t i = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
			image_8u.at<uint8_t>(r, c) = i++;

	image_converter::convert_8u_32f( image_8u, image_32f );

	float f = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
			EXPECT_FLOAT_EQ( image_32f.at<float>(r, c), f++ );
}

TEST( image_converter_tests, convert_8u_32f_c3_works )
{
	auto image_8u = Mat( 3, 3, CV_8UC3 );
	auto image_32f = Mat( 3, 3, CV_32FC3 );

	uint8_t i = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
		{
			image_8u.at<Vec3b>(r, c)[0] = i++;
			image_8u.at<Vec3b>(r, c)[1] = i++;
			image_8u.at<Vec3b>(r, c)[2] = i++;
		}

	image_converter::convert_8u_32f( image_8u, image_32f );

	float f = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
		{
			EXPECT_FLOAT_EQ( image_32f.at<Vec3f>(r, c)[0], f++ );
			EXPECT_FLOAT_EQ( image_32f.at<Vec3f>(r, c)[1], f++ );
			EXPECT_FLOAT_EQ( image_32f.at<Vec3f>(r, c)[2], f++ );
		}
}

TEST( image_converter_tests, convert_32f_8u_different_size_throws_exception )
{
    {
        auto image_32f = Mat( 3, 3, CV_32FC1 );
        auto image_8u = Mat( 4, 4, CV_8UC1 );

        EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u, { 4, 3 } ), argument_exception );
        EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u, { 3, 4 } ), argument_exception );
    }

    {
        auto image_32f = Mat( 4, 4, CV_32FC1 );
        auto image_8u = Mat( 3, 3, CV_8UC1 );

        EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u, { 4, 3 } ), argument_exception );
        EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u, { 3, 4 } ), argument_exception );
    }
}

TEST( image_converter_tests, convert_32f_8u_different_channels_throws_exception )
{
    auto image_32f = Mat( 3, 3, CV_32FC3 );
    auto image_8u = Mat( 3, 3, CV_8UC1 );

    EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
}

TEST( image_converter_tests, convert_32f_8u_wrong_format_throws_exception )
{
	{
		auto image_32f = Mat( 3, 3, CV_8UC1 );
		auto image_8u = Mat( 3, 3, CV_8UC1 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}

	{
		auto image_32f = Mat( 3, 3, CV_32FC1 );
		auto image_8u = Mat( 3, 3, CV_32FC1 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}
}

TEST( image_converter_tests, convert_32f_8u_unsupported_channels_throws_exception )
{
	{
		auto image_32f = Mat( 3, 3, CV_32FC2 );
		auto image_8u = Mat( 3, 3, CV_8UC2 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}

	{
		auto image_32f = Mat( 3, 3, CV_32FC4 );
		auto image_8u = Mat( 3, 3, CV_8UC4 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}
}

TEST( image_converter_tests, convert_32f_8u_c1_works )
{
	auto image_32f = Mat( 3, 3, CV_32FC1 );
	auto image_8u = Mat( 3, 3, CV_8UC1 );

	float f = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
			image_32f.at<float>(r, c) = f++;

	image_converter::convert_32f_8u( image_32f, image_8u );

	uint8_t i = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
			EXPECT_EQ( image_8u.at<uint8_t>(r, c), i++ );
}

TEST( image_converter_tests, convert_32f_8u_c3_works )
{
	auto image_32f = Mat( 3, 3, CV_32FC3 );
	auto image_8u = Mat( 3, 3, CV_8UC3 );

	float f = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
		{
			image_32f.at<Vec3f>(r, c)[0] = f++;
			image_32f.at<Vec3f>(r, c)[1] = f++;
			image_32f.at<Vec3f>(r, c)[2] = f++;
		}

	image_converter::convert_32f_8u( image_32f, image_8u );

	uint8_t i = 0;
	for( int r = 0; r < 3; r++ )
		for( int c = 0; c < 3; c++ )
		{
			EXPECT_EQ( image_8u.at<Vec3b>(r, c)[0], i++ );
			EXPECT_EQ( image_8u.at<Vec3b>(r, c)[1], i++ );
			EXPECT_EQ( image_8u.at<Vec3b>(r, c)[2], i++ );
		}
}

TEST( image_converter_tests, convert_8u_32fc_only_one_channel_supported )
{
    auto image_8u = Mat( 3, 3, CV_8UC3 );
    auto image_32fc = dh::image_32fc( 3, 3 );
    EXPECT_THROW( image_converter::convert_8u_32fc( image_8u, image_32fc ), argument_exception );
}

TEST( image_converter_tests, convert_8u_32fc_wrong_depth_throws_exception )
{
    auto image_8u = Mat( 3, 3, CV_16UC1 );
    auto image_32fc = dh::image_32fc( 3, 3 );
    EXPECT_THROW( image_converter::convert_8u_32fc( image_8u, image_32fc ), argument_exception );
}

TEST( image_converter_tests, convert_8u_32fc_different_size_throws_exception )
{
    auto image_8u = Mat( 3, 3, CV_8UC1 );

    {
        auto image_32fc = dh::image_32fc( 3, 4 );
        EXPECT_THROW( image_converter::convert_8u_32fc( image_8u, image_32fc ), argument_exception );
    }

    {
        auto image_32fc = dh::image_32fc( 4, 3 );
        EXPECT_THROW( image_converter::convert_8u_32fc( image_8u, image_32fc ), argument_exception );
    }
}

TEST( image_converter_tests, convert_8u_32fc_works )
{
    auto image_8u = Mat( 3, 3, CV_8UC1 );
    auto image_32fc = dh::image_32fc( 3, 3 );

    uint8_t i = 0;
    for( int r = 0; r < 3; r++ )
        for( int c = 0; c < 3; c++ )
            image_8u.at<uint8_t>(r, c) = i++;

    image_converter::convert_8u_32fc( image_8u, image_32fc );

    float f = 0;
    for( int y = 0; y < 3; y++ )
        for( int x = 0; x < 3; x++ )
        {
            EXPECT_FLOAT_EQ( image_32fc.at(x, y).re, f++ );
            EXPECT_FLOAT_EQ( image_32fc.at(x, y).im, 0.0f );
        }
}

TEST( image_converter_tests, separate_32fc_only_one_channel_supported )
{
    auto image_32fc = dh::image_32fc( 3, 3 );

    auto real_32f_c1 = Mat( 3, 3, CV_32FC1 );
    auto imag_32f_c1 = Mat( 3, 3, CV_32FC1 );

    auto real_32f_c3 = Mat( 3, 3, CV_32FC3 );
    auto imag_32f_c3 = Mat( 3, 3, CV_32FC3 );

    EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f_c3, imag_32f_c1 ), argument_exception );
    EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f_c1, imag_32f_c3 ), argument_exception );
}

TEST( image_converter_tests, separate_32fc_invalid_depth_throws_exception )
{
    auto image_32fc = dh::image_32fc( 3, 3 );

    auto real_32f = Mat( 3, 3, CV_32FC1 );
    auto imag_32f = Mat( 3, 3, CV_32FC1 );

    auto real_64f = Mat( 3, 3, CV_64FC1 );
    auto imag_64f = Mat( 3, 3, CV_64FC1 );

    EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_64f, imag_32f ), argument_exception );
    EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f, imag_64f ), argument_exception );
}

TEST( image_converter_tests, separate_32fc_different_size_throws_exception )
{
    auto image_32fc = dh::image_32fc( 3, 3 );

    auto real_32f = Mat( 3, 3, CV_32FC1 );
    auto imag_32f = Mat( 3, 3, CV_32FC1 );

    {
        auto real_32f_invalid_width = Mat( 3, 4, CV_32FC1 );
        auto real_32f_invalid_height = Mat( 4, 3, CV_32FC1 );

        EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f_invalid_width, imag_32f ),
                      argument_exception );

        EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f_invalid_height, imag_32f ),
                      argument_exception );
    }

    {
        auto imag_32f_invalid_width = Mat( 3, 4, CV_32FC1 );
        auto imag_32f_invalid_height = Mat( 4, 3, CV_32FC1 );

        EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f, imag_32f_invalid_width ),
                      argument_exception );

        EXPECT_THROW( image_converter::separate_32fc( image_32fc, real_32f, imag_32f_invalid_height ),
                      argument_exception );
    }
}

TEST( image_converter_tests, separate_32fc_works )
{
    auto image_32fc = dh::image_32fc( 3, 3 );

    auto real_32f = Mat( 3, 3, CV_32FC1 );
    auto imag_32f = Mat( 3, 3, CV_32FC1 );

    {
        float f = 0;
        for( int r = 0; r < 3; r++ )
            for( int c = 0; c < 3; c++ )
            {
                image_32fc.at(c, r) = { f, 10*f };
                f++;
            }
    }

    image_converter::separate_32fc( image_32fc, real_32f, imag_32f );

    float f = 0;
    for( int r = 0; r < 3; r++ )
        for( int c = 0; c < 3; c++ )
        {
            EXPECT_FLOAT_EQ( real_32f.at<float>(r, c), f );
            EXPECT_FLOAT_EQ( imag_32f.at<float>(r, c), 10*f );
            f++;
        }
}
