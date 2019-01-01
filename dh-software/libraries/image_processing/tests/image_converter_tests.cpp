#include "image_converter.h"

#include "gtest/gtest.h"

using namespace cv;
using namespace testing;

TEST( image_converter_tests, convert_8u_32f_different_size_throws_exception )
{
	{
		auto image_8u = Mat( 3, 3, CV_8UC1 );
		auto image_32f = Mat( 2, 3, CV_32FC1 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}

	{
		auto image_8u = Mat( 3, 3, CV_8UC1 );
		auto image_32f = Mat( 3, 2, CV_32FC1 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}

	{
		auto image_8u = Mat( 3, 3, CV_8UC1 );
		auto image_32f = Mat( 3, 3, CV_32FC3 );

		EXPECT_THROW( image_converter::convert_8u_32f( image_8u, image_32f ), argument_exception );
	}
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
		auto image_8u = Mat( 2, 3, CV_8UC1 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}

	{
		auto image_32f = Mat( 3, 3, CV_32FC1 );
		auto image_8u = Mat( 3, 2, CV_8UC1 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}

	{
		auto image_32f = Mat( 3, 3, CV_32FC1 );
		auto image_8u = Mat( 2, 3, CV_8UC3 );

		EXPECT_THROW( image_converter::convert_32f_8u( image_32f, image_8u ), argument_exception );
	}
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
