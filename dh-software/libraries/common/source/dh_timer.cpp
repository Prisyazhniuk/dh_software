#include "dh_timer.h"

#include <thread>

namespace dh
{
    uint64_t dh_timer::now_us()
    {
        auto duration = std::chrono::steady_clock::now().time_since_epoch();
        auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>( duration );
        return uint64_t( duration_us.count() );
    }

    uint64_t dh_timer::now_ms()
    {
        auto duration = std::chrono::steady_clock::now().time_since_epoch();
        auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>( duration );
        return uint64_t( duration_ms.count() );
    }

    uint64_t dh_timer::now_ns()
    {
        auto duration = std::chrono::steady_clock::now().time_since_epoch();
        auto duration_ms = std::chrono::duration_cast<std::chrono::nanoseconds>( duration );
        return uint64_t( duration_ms.count() );
    }

    void dh_timer::wait_for_ms( uint64_t ms )
    {
        std::this_thread::sleep_for( std::chrono::milliseconds( ms ) );
    }

    void dh_timer::wait_for_ms( const std::chrono::milliseconds& value )
    {
        std::this_thread::sleep_for( value );
    }

    void dh_timer::wait_for_us( uint64_t us )
    {
        std::this_thread::sleep_for( std::chrono::microseconds( us ) );
    }

    void dh_timer::wait_for_us( const std::chrono::microseconds& value )
    {
        std::this_thread::sleep_for( value );
    }
}
