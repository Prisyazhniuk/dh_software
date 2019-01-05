#include "dh_string.h"

#include "gtest/gtest.h"

using namespace testing;

TEST( dh_string_tests, fs )
{
	EXPECT_EQ( "string", dh_string::fs( "string" ) );

	EXPECT_EQ( "string substring", dh_string::fs( "string %s", "substring" ) );

	EXPECT_EQ( "_5_", dh_string::fs( "_%d_", 5 ) );

	EXPECT_EQ( "_5.12_", dh_string::fs( "_%0.2f_", 5.1234 ) );

	EXPECT_EQ( "0a 0b 0c", dh_string::fs( "%02x %02x %02x", 10, 11, 12 ) );
}
