#pragma once

#include "dh_thread.h"

#include <atomic>
#include <string>

class fft_viewer_core
{
public:
	fft_viewer_core();
	~fft_viewer_core();

	void run( const std::string& file_name );

private:
	void processing_thread();

private:
	std::atomic_bool _shutdown_signal;
	dh_thread _processing_thread;
};
