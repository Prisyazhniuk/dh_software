#include "hologram_processor.h"
#include "image_converter.h"
#include "gabor.h"
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
    hologram_processor::~hologram_processor()
    {
        stop();
    }

    void hologram_processor::reconstruct( const string& image_path )
    {
        stop();

        _processing_thread = dh_thread( "hologram_processor thread",
                                        &hologram_processor::processing_thread,
                                        this,
                                        image_path );
    }

    void hologram_processor::stop()
    {
        if( _processing_thread.joinable() )
            _processing_thread.join();
    }

    void hologram_processor::processing_thread( const string& image_path )
    {
        float lambda = 0.0006328f;
        float sensor_width = 4.24f;
        //float sensor_height = 2.39f;
        float distance = 23.2f;
        //float theta = 0.0f;

        auto frame_processing_start_time = dh_timer::now_us();

        auto hologram_8u = imread( image_path.c_str(), IMREAD_GRAYSCALE );
        if( hologram_8u.empty() )
        {
            emit error( "Ошибка загрузки изображения" );
            return;
        }

        const auto width = hologram_8u.cols;
        const auto height = hologram_8u.rows;

        const float pixel_size = sensor_width / width;


        auto kernel_32fc = image_32fc( width, height );
        auto kernel_fft_32fc = image_32fc( width, height );

        auto hologram_32fc = image_32fc( width, height );
        auto hologram_fft_32fc = image_32fc( width, height );

        auto object_32fc = image_32fc( width, height );
        auto object_fft_32fc = image_32fc( width, height );

        auto object_32f = Mat( height, width, CV_32FC1 );
        auto object_8u = Mat( height, width, CV_8UC1 );

        dft dft( width, height );
        spectrum_shifter spectrum_shifter( int( object_8u.step ), height, object_8u.channels() );


        double max_src = 0;
        minMaxLoc( hologram_8u, nullptr, &max_src );

        image_converter::convert_8u_32fc( hologram_8u, hologram_32fc );

        auto c = Ipp32fc{ float(max_src), 0 };
        auto status = ippsMulC_32fc_I( c, hologram_32fc.data(), hologram_32fc.height() * hologram_32fc.step_in_elements() );

        if( status != ippStsNoErr )
        {
            emit error( QString("ippsMulC_32fc_I() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        dft.forward( hologram_32fc, hologram_fft_32fc );


        gabor::make_kernel( kernel_32fc, lambda, distance, pixel_size );
        dft.forward( kernel_32fc, kernel_fft_32fc );


        status = ippsMul_32fc( hologram_fft_32fc.data(), kernel_fft_32fc.data(), object_fft_32fc.data(),
                               hologram_fft_32fc.height() * hologram_fft_32fc.step_in_elements() );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippsMul_32fc() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        dft.inverse( object_fft_32fc, object_32fc );


        status = ippiMagnitude_32fc32f_C1R( object_32fc.data(), object_32fc.step_in_bytes(),
                                            object_32f.ptr<float>(), int( object_32f.step ),
                                            { width, height } );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiMagnitude_32fc32f_C1R() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        float min, max;
        status = ippiMinMax_32f_C1R( object_32f.ptr<float>(),
                                     static_cast<int>( object_32f.step ),
                                     { width, height }, &min, &max );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippiMinMax_32f_C1R() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        const float sub = min;
        const float div = min-max == 0.0f ? 1 : ( min-max ) / 255.0f;

        status = ippsNormalize_32f_I( object_32f.ptr<float>(),
                                      height * static_cast<int>( object_32f.step / sizeof(float) ),
                                      sub, div );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippsNormalize_32f_I() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        status = ippsAddC_32f_I( 255.0f, object_32f.ptr<float>(),
                                 height * static_cast<int>( object_32f.step / sizeof(float) ) );
        if( status != ippStsNoErr )
        {
            emit error( QString("ippsAddC_32f_I() error: %1").arg( ippGetStatusString( status ) ) );
            return;
        }

        image_converter::convert_32f_8u( object_32f, object_8u );

        spectrum_shifter.shift( object_8u );

        auto magnitudes_q = QImage( object_8u.data, object_8u.cols, object_8u.rows,
                                    static_cast<int>( object_8u.step ), QImage::Format_Grayscale8 );

        emit image_processed( magnitudes_q.copy() );

        processing_statistics statistics;
        statistics.time_us = dh_timer::now_us() - frame_processing_start_time;
        emit statistics_ready( statistics );
    }
}
