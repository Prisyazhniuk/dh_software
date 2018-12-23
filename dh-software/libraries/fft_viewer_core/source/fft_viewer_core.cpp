#include "fft_viewer_core.h"

#include <opencv2/imgcodecs.hpp>

#include <iostream>

using namespace cv;
using namespace std;

fft_viewer_core::fft_viewer_core()
	: _shutdown_signal( false )
{}

fft_viewer_core::~fft_viewer_core()
{
	_shutdown_signal = true;

	if( _processing_thread.joinable() )
		_processing_thread.join();
}

void fft_viewer_core::run( const std::string& file_name )
{
	auto image = imread( file_name, IMREAD_COLOR );

	cout << image.cols << "x" << image.rows << endl;
}

void fft_viewer_core::processing_thread()
{

}
