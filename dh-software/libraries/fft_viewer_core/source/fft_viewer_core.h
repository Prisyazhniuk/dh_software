#pragma once

#include "dh_thread.h"

#include <QPixmap>

#include <atomic>
#include <string>

class fft_viewer_core : public QObject
{
    Q_OBJECT

public:
	fft_viewer_core();
	~fft_viewer_core();

    void run( const std::string& image_path );

signals:
    void image_processed( const QPixmap& );


private:
    void processing_thread( const std::string& image_path );

private:
	std::atomic_bool _shutdown_signal;
	dh_thread _processing_thread;
};
