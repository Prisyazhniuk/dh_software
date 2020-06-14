#include "hologram_processor.h"
#include "image_converter.h"
#include "gabor.h"
#include "dft.h"
#include "spectrum_shifter.h"
#include "dh_timer.h"

#include <ippi.h>
#include <ipps.h>

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

    void hologram_processor::reconstruct( const Mat& hologram_8u, const processing_settings& settings )
    {
        if( hologram_8u.type() != CV_8UC1 )
        {
            emit error( "Неподдерживаемый формат изображения" );
            return;
        }

        stop();

        _processing_thread = dh_thread( "hologram_processor thread",
                                        &hologram_processor::processing_thread,
                                        this, hologram_8u.clone(), settings );
    }

    void hologram_processor::stop()
    {
        if( _processing_thread.joinable() )
            _processing_thread.join();
    }

    void hologram_processor::processing_thread( const Mat& hologram_8u, const processing_settings& settings )
    {
        try
        {
            auto frame_processing_start_time = dh_timer::now_us();

            const auto width = hologram_8u.cols;
            const auto height = hologram_8u.rows;

            const float pixel_size_w = settings.sensor_width_mm / width;
            const float pixel_size_h = settings.sensor_height_mm / height;

            if( abs( pixel_size_h - pixel_size_w ) > 0.00005f )
                throw QString("Размер пикселя по ширине и высоте не совпадает");

            auto kernel_32fc = image_32fc( width, height );
            auto kernel_fft_32fc = image_32fc( width, height );

            auto hologram_32fc = image_32fc( width, height );
            auto hologram_fft_32fc = image_32fc( width, height );

            auto object_32fc = image_32fc( width, height );
            auto object_fft_32fc = image_32fc( width, height );

            auto magnitudes_32f = Mat( height, width, CV_32FC1 );
            auto magnitudes_8u = Mat( height, width, CV_8UC1 );

            auto phase_32f = Mat( height, width, CV_32FC1 );
            auto phase_8u = Mat( height, width, CV_8UC1 );

            auto amplitude_32f = Mat( height, width, CV_32FC1 );
            auto amplitude_8u = Mat( height, width, CV_8UC1 );

            auto intensity_32f = Mat( height, width, CV_32FC1 );
            auto intensity_8u = Mat( height, width, CV_8UC1 );

            image_converter::convert_8u_32fc( hologram_8u, hologram_32fc );

            uint8_t max_src;
            auto status = ippiMax_8u_C1R( hologram_8u.data,
                                             int( hologram_8u.step ),
                                             { width, height },
                                             &max_src );
            if( status != ippStsNoErr )
                throw QString("ippiMax_8u_C1R() error: %1").arg( ippGetStatusString( status ) );

            auto c = Ipp32fc{ float(max_src), 0 };

            status = ippsMulC_32fc_I( c,
                                      hologram_32fc.data(),
                                      hologram_32fc.height() * hologram_32fc.step_in_elements() );
            if( status != ippStsNoErr )
                throw QString("ippsMulC_32fc_I() error: %1").arg( ippGetStatusString( status ) );

            dft dft( width, height );
            dft.forward( hologram_32fc, hologram_fft_32fc );

            gabor::make_kernel( kernel_32fc, settings.lambda_mm, settings.distance_mm, pixel_size_w );
            dft.forward( kernel_32fc, kernel_fft_32fc );

            status = ippsMul_32fc( hologram_fft_32fc.data(),
                                   kernel_fft_32fc.data(),
                                   object_fft_32fc.data(),
                                   hologram_fft_32fc.height() * hologram_fft_32fc.step_in_elements() );
            if( status != ippStsNoErr )
                throw QString("ippsMul_32fc() error: %1").arg( ippGetStatusString( status ) );

            dft.inverse( object_fft_32fc, object_32fc );

            spectrum_shifter spectrum_shifter( object_32fc.width(), object_32fc.height() );
            spectrum_shifter.shift( object_32fc );

            status = ippiPhase_32fc32f_C1R( object_32fc.data(),
                                            object_32fc.step_in_bytes(),
                                            phase_32f.ptr<float>(),
                                            int( phase_32f.step ),
                                            { width, height } );
            if( status != ippStsNoErr )
                throw QString("ippiPhase_32fc32f_C1R() error: %1").arg( ippGetStatusString( status ) );

            status = ippiMagnitude_32fc32f_C1R( object_32fc.data(),
                                                object_32fc.step_in_bytes(),
                                                magnitudes_32f.ptr<float>(),
                                                int( magnitudes_32f.step ),
                                                { width, height } );
            if( status != ippStsNoErr )
                throw QString("ippiMagnitude_32fc32f_C1R() error: %1").arg( ippGetStatusString( status ) );

            status = ippiSqr_32f_C1R( magnitudes_32f.ptr<float>(),
                                      int(magnitudes_32f.step),
                                      intensity_32f.ptr<float>(),
                                      int(intensity_32f.step),
                                      { width, height } );
            if( status != ippStsNoErr )
                throw QString("ippiSqr_32f_C1R() error: %1").arg( ippGetStatusString( status ) );

            normalize_32f( phase_32f );
            image_converter::convert_32f_8u( phase_32f, phase_8u );

            normalize_32f( magnitudes_32f );
            image_converter::convert_32f_8u( magnitudes_32f, magnitudes_8u );

            normalize_32f( intensity_32f );
            image_converter::convert_32f_8u( intensity_32f, intensity_8u );

            auto results = processing_results
            {
                .original = hologram_8u,
                .phase = phase_8u,
                .amplitude = magnitudes_8u,
                .intensity = intensity_8u,
                .real = nullopt,
                .imaginary = nullopt
            };

            emit processed( results );

            auto statistics = processing_statistics
            {
                .time_us = dh_timer::now_us() - frame_processing_start_time
            };

            emit statistics_ready( statistics );
        }
        catch( QString& error_message )
        {
            emit error( error_message );
        }
    }

    void hologram_processor::normalize_32f( Mat& m )
    {
        const auto width = m.cols;
        const auto height = m.rows;

        float min, max;
        auto status = ippiMinMax_32f_C1R( m.ptr<float>(),
                                          int( m.step ),
                                          { width, height },
                                          &min,
                                          &max );
        if( status != ippStsNoErr )
            throw QString("ippiMinMax_32f_C1R() error: %1").arg( ippGetStatusString( status ) );

        const float sub = min;
        const float div = max-min == 0.0f ? 1 : ( max-min ) / 255.0f;

        status = ippsNormalize_32f_I( m.ptr<float>(),
                                      height * int( m.step / sizeof(float) ),
                                      sub, div );
        if( status != ippStsNoErr )
            throw QString("ippsNormalize_32f_I() error: %1").arg( ippGetStatusString( status ) );
    }
}
