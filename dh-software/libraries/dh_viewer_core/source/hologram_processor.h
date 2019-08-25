#pragma once

#include "error_reporter.h"
#include "processing_settings.h"
#include "processing_statistics.h"
#include "dh_thread.h"

#include <QImage>

#include <atomic>
#include <string>

namespace dh
{
    class hologram_processor : public error_reporter
    {
        Q_OBJECT

    public:
        ~hologram_processor();

        void reconstruct( const std::string& image_path, const processing_settings& settings );
        void stop();

    signals:
        void image_processed( const QImage& );
        void statistics_ready( const processing_statistics& );

    private:
        void processing_thread( const std::string& image_path, const processing_settings& settings );

    private:
        dh_thread _processing_thread;        
    };
}
