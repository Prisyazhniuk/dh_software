#pragma once

#include "blob_detection_statistics.h"
#include "dh_thread.h"

#include <QImage>

#include <atomic>
#include <string>

namespace dh
{
    class blob_detector : public QObject
    {
        Q_OBJECT

    public:
        ~blob_detector();

        void run( const std::string& image_path );
        void stop();

    signals:
        void image_processed( const QImage& );
        void statistics_ready( const blob_detection_statistics& );

    private:
        void processing_thread( const std::string& image_path );

    private:
        dh_thread _processing_thread;
    };
}
