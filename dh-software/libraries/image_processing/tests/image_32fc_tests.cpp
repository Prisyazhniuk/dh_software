#include "image_processing.h"
#include "image_32fc.h"

#include <gtest/gtest.h>

using namespace dh;
using namespace testing;

TEST( image_32fc_tests, constructor_not_positive_sizes_throws_exception )
{
    EXPECT_THROW( image_32fc( 0, 1 ), argument_exception );
    EXPECT_THROW( image_32fc( 1, 0 ), argument_exception );
}

TEST( image_32fc_tests, sizes_is_correct )
{
    image_32fc image( 10, 15 );
    EXPECT_EQ( 10, image.width() );
    EXPECT_EQ( 15, image.height() );
}

TEST( image_32fc_tests, step_is_correct )
{
    {
        image_32fc image( 1, 15 );
        EXPECT_EQ( 64, image.step() );
    }

    {
        image_32fc image( 9, 15 );
        EXPECT_EQ( 128, image.step() );
    }
}

TEST( image_32fc_tests, data_is_correct )
{
    image_32fc image( 10, 10 );
    EXPECT_TRUE( image.data() != nullptr );
}
