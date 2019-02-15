#pragma once

#include <chrono>

namespace dh
{
    class dh_timer
    {
    public:
        uint64_t now_us();
        uint64_t now_ms();
        uint64_t now_ns();

        void wait_for_ms( uint64_t ms );
        void wait_for_ms( const std::chrono::milliseconds& );

        void wait_for_us( uint64_t us );
        void wait_for_us( const std::chrono::microseconds& );
    };
}
