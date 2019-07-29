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

TEST( image_32fc_tests, step_in_bytes_is_correct )
{
    {
        image_32fc image( 1, 15 );
        EXPECT_EQ( 64, image.step_in_bytes() );
    }

    {
        image_32fc image( 9, 15 );
        EXPECT_EQ( 128, image.step_in_bytes() );
    }
}

TEST( image_32fc_tests, step_in_elements_is_correct )
{
    {
        image_32fc image( 1, 15 );
        auto expected_step = int( 64 / sizeof(Ipp32fc) );
        EXPECT_EQ( expected_step, image.step_in_elements() );
    }

    {
        image_32fc image( 9, 15 );
        auto expected_step = int( 128 / sizeof(Ipp32fc) );
        EXPECT_EQ( expected_step, image.step_in_elements() );
    }
}


TEST( image_32fc_tests, at_works )
{
    image_32fc image( 2, 2 );

    image.at( 0, 0 ) = { 1, 11 };
    image.at( 1, 0 ) = { 2, 22 };
    image.at( 0, 1 ) = { 3, 33 };
    image.at( 1, 1 ) = { 4, 44 };

    auto data = image.data();
    EXPECT_FLOAT_EQ( 1, data->re );
    EXPECT_FLOAT_EQ( 11, data->im );

    data++;
    EXPECT_FLOAT_EQ( 2, data->re );
    EXPECT_FLOAT_EQ( 22, data->im );

    data = image.data() + size_t(image.step_in_bytes()) / sizeof(Ipp32fc);
    EXPECT_FLOAT_EQ( 3, data->re );
    EXPECT_FLOAT_EQ( 33, data->im );

    data++;
    EXPECT_FLOAT_EQ( 4, data->re );
    EXPECT_FLOAT_EQ( 44, data->im );
}

TEST( image_32fc_tests, data_is_correct )
{
    image_32fc image( 10, 10 );
    EXPECT_TRUE( image.data() != nullptr );
}
