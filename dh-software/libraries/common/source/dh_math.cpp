#include "dh_math.h"

namespace dh
{
    int dh_math::power_of_2( uint32_t value, uint32_t& next_value )
    {
        int power = 0;
        next_value = 1;

        while( next_value < value )
        {
            power++;
            next_value <<= 1;
        }

        return power;
    }
}
