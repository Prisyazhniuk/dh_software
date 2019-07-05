#pragma once

#include "error_reporter.h"
#include "fft_processing_statistics.h"
#include "dh_thread.h"

#include <QImage>

#include <atomic>
#include <string>

namespace dh
{
    class fft_processor : public error_reporter
    {
        Q_OBJECT

    public:
        ~fft_processor();

        void run( const std::string& image_path );
        void stop();

    signals:
        void image_processed( const QImage& );
        void statistics_ready( const fft_processing_statistics& );

    private:
        void processing_thread( const std::string& image_path );

    private:
        static const int _statistics_collection_period_us = 1000*1000;

        static const int _max_fps = 50;
        static const int _min_processing_time_us = 1000*1000/_max_fps;

        dh_thread _processing_thread;        
    };
}
