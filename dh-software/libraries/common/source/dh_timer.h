#pragma once

#include <chrono>

namespace dh
{
    class dh_timer
    {
    public:
        static uint64_t now_us();
        static uint64_t now_ms();
        static uint64_t now_ns();

        static void wait_for_ms( uint64_t ms );
        static void wait_for_ms( const std::chrono::milliseconds& );

        static void wait_for_us( uint64_t us );
        static void wait_for_us( const std::chrono::microseconds& );
    };
}
