#include "fft_processor.h"
#include "image_converter.h"
#include "dft.h"
#include "spectrum_shifter.h"
#include "dh_timer.h"

#include <ippi.h>
#include <ipps.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

namespace dh
{
    fft_processor::~fft_processor()
    {
        stop();
    }

    void fft_processor::run( const string& image_path )
    {
        stop();

        _processing_thread = dh_thread( "fft_processor thread",
                                        &fft_processor::processing_thread,
                                        this,
                                        image_path );
    }

    void fft_processor::stop()
    {
        if( _processing_thread.joinable() )
            _processing_thread.join();
    }

    void fft_processor::processing_thread( const string& image_path )
    {
        auto image_8u = imread( image_path.c_str(), IMREAD_GRAYSCALE );
        if( image_8u.empty() )
        {
            emit error( "Ошибка загрузки изображения" );
            return;
        }        

        auto rows = image_8u.rows;
        auto cols = image_8u.cols;

        auto image_32f = Mat( rows, cols, CV_32FC1 );
        auto magnitudes_32f = Mat( rows, cols, CV_32FC1 );
        auto magnitudes_8u = Mat( rows, cols, CV_8UC1 );

        dft dft( cols, rows, image_8u.channels() );
        spectrum_shifter spectrum_shifter( int( magnitudes_8u.step ), rows, magnitudes_8u.channels() );

        auto frame_processing_start_time = dh_timer::now_us();

        image_converter::convert_8u_32f( image_8u, image_32f );

        dft.forward( image_32f, magnitudes_32f );

        auto status = ippiLn_32f_C1IR( magnitudes_32f.ptr<float>(),
                                       static_cast<int>( magnitudes_32f.step ),
                                       {cols, rows} );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiLn_32f_C1IR() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        float min, max;
        status = ippiMinMax_32f_C1R( magnitudes_32f.ptr<float>(),
                                     static_cast<int>( magnitudes_32f.step ),
                                     {cols, rows}, &min, &max );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiMinMax_32f_C1R() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        float sub = min;
        float div = ( max-min ) / 255.0f;
        status = ippsNormalize_32f_I( magnitudes_32f.ptr<float>(),
                                      rows * static_cast<int>( magnitudes_32f.step / sizeof(float) ),
                                      sub, div );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippsNormalize_32f_I() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        image_converter::convert_32f_8u( magnitudes_32f, magnitudes_8u );
        spectrum_shifter.shift( magnitudes_8u );

        auto magnitudes_q = QImage( magnitudes_8u.data, magnitudes_8u.cols, magnitudes_8u.rows,
                                    static_cast<int>( magnitudes_8u.step ), QImage::Format_Grayscale8 );

        emit image_processed( magnitudes_q.copy() );

        fft_processing_statistics statistics;
        statistics.time_us = dh_timer::now_us() - frame_processing_start_time;
        emit statistics_ready( statistics );
    }
}
