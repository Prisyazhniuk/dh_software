#include "dh_math.h"

#include "gtest/gtest.h"

using namespace testing;

TEST( dh_math_tests, next_power_of_2 )
{
	uint32_t next;

	EXPECT_EQ( 0, dh_math::power_of_2( 0, next ) );
	EXPECT_EQ( 1u, next );

	EXPECT_EQ( 0, dh_math::power_of_2( 1, next ) );
	EXPECT_EQ( 1u, next );

	EXPECT_EQ( 1, dh_math::power_of_2( 2, next ) );
	EXPECT_EQ( 2u, next );

	EXPECT_EQ( 2, dh_math::power_of_2( 3, next ) );
	EXPECT_EQ( 4u, next );

	EXPECT_EQ( 7, dh_math::power_of_2( 127, next ) );
	EXPECT_EQ( 128u, next );
}
