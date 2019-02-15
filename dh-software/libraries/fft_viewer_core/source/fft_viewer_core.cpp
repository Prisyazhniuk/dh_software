#include "fft_viewer_core.h"
#include "image_converter.h"
#include "dft.h"

// TODO
#include <QDebug>
#include <ippi.h>
#include <ipps.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

fft_viewer_core::fft_viewer_core()
	: _shutdown_signal( false )
{}

fft_viewer_core::~fft_viewer_core()
{
    stop();
}

void fft_viewer_core::run_processing( const string& image_path )
{
    stop();

    _processing_thread = dh_thread( "processing_thread",
                                    &fft_viewer_core::processing_thread,
                                    this,
                                    image_path );
}

void fft_viewer_core::stop()
{
    _shutdown_signal = true;

    if( _processing_thread.joinable() )
        _processing_thread.join();

    _shutdown_signal = false;
}

void fft_viewer_core::processing_thread( const string& image_path )
{
    auto image_8u = imread( image_path.c_str(), IMREAD_GRAYSCALE );
    if( image_8u.empty() )
    {
        qCritical() << "image open error";
        return;
    }

    auto rows = image_8u.rows;
    auto cols = image_8u.cols;

    auto image_32f = Mat( rows, cols, CV_32FC1 );
    auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );
    auto magnitudes_8u = Mat( rows, cols, CV_8UC1 );

    ::dft dft( cols, rows, image_8u.channels() );

    while( !_shutdown_signal )
    {
        image_converter::convert_8u_32f( image_8u, image_32f );

        dft.forward( image_32f, magnitudes_32f );

        auto status = ippiLn_32f_C1IR( magnitudes_32f.ptr<float>(),
                                       static_cast<int>( magnitudes_32f.step ),
                                       {cols, rows} );
        if( status != ippStsNoErr )
            // throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
            qCritical() << ippGetStatusString( status );

        float min, max;
        status = ippiMinMax_32f_C1R( magnitudes_32f.ptr<float>(),
                                     static_cast<int>( magnitudes_32f.step ),
                                     {cols, rows}, &min, &max );
        if( status != ippStsNoErr )
            qCritical() << ippGetStatusString( status );

        float sub = min;
        float div = ( max-min ) / 255.0f;
        status = ippsNormalize_32f_I( magnitudes_32f.ptr<float>(),
                                      rows * static_cast<int>( magnitudes_32f.step / sizeof(float) ),
                                      sub, div );
        if( status != ippStsNoErr )
            qCritical() << ippGetStatusString( status );

        image_converter::convert_32f_8u( magnitudes_32f, magnitudes_8u );

        // cvtColor( image_8u, image_8u, COLOR_BGR2RGB );

        auto magnitudes_q = QImage( magnitudes_8u.data, magnitudes_8u.cols, magnitudes_8u.rows,
                                    static_cast<int>( magnitudes_8u.step ), QImage::Format_Grayscale8 );

        auto pixmap = QPixmap::fromImage( magnitudes_q );

        emit image_processed( pixmap );
    }
}
