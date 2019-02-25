#pragma once

#include "processing_statistics.h"
#include "dh_thread.h"

#include <QImage>

#include <atomic>
#include <string>

namespace dh
{
    class fft_viewer_core : public QObject
    {
        Q_OBJECT

    public:
        fft_viewer_core();
        ~fft_viewer_core();

        void run( const std::string& image_path );
        void stop();

    signals:
        void image_processed( const QImage& );
        void statistics_ready( const processing_statistics& );

    private:
        void processing_thread( const std::string& image_path );

    private:
        static const int _statistics_collection_period_us = 1000*1000;

        static const int _max_fps = 50;
        static const int _min_processing_time_us = 1000*1000/_max_fps;

        std::atomic_bool _shutdown_signal;
        dh_thread _processing_thread;

        processing_statistics _statistics;
    };
}
