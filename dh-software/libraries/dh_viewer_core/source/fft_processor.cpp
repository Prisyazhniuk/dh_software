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
        auto src_8u = imread( image_path.c_str(), IMREAD_GRAYSCALE );
        if( src_8u.empty() )
        {
            emit error( "Ошибка загрузки изображения" );
            return;
        }        

        auto width = src_8u.cols;
        auto height = src_8u.rows;


        auto src_32fc = image_32fc( width, height );
        auto magnitudes_32fc = image_32fc( width, height );
        auto magnitudes_32f = Mat( height, width, CV_32FC1 );
        auto magnitudes_8u = Mat( height, width, CV_8UC1 );

        dft dft( width, height );
        spectrum_shifter spectrum_shifter( int( magnitudes_8u.step ), height, magnitudes_8u.channels() );

        auto frame_processing_start_time = dh_timer::now_us();

        image_converter::convert_8u_32fc( src_8u, src_32fc );

        dft.forward( src_32fc, magnitudes_32fc );

        auto status = ippiMagnitude_32fc32f_C1R( magnitudes_32fc.data(), magnitudes_32fc.step_in_bytes(),
                                                 magnitudes_32f.ptr<float>(), int( magnitudes_32f.step ),
                                                 { width, height } );
        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );

        status = ippiLn_32f_C1IR( magnitudes_32f.ptr<float>(), static_cast<int>( magnitudes_32f.step ),
                                  { width, height } );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiLn_32f_C1IR() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        float min, max;
        status = ippiMinMax_32f_C1R( magnitudes_32f.ptr<float>(),
                                     static_cast<int>( magnitudes_32f.step ),
                                     { width, height }, &min, &max );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiMinMax_32f_C1R() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        float sub = min;
        float div = ( max-min ) / 255.0f;
        status = ippsNormalize_32f_I( magnitudes_32f.ptr<float>(),
                                      height * static_cast<int>( magnitudes_32f.step / sizeof(float) ),
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
