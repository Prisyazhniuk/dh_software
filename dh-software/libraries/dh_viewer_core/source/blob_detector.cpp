#include "blob_detector.h"
#include "image_converter.h"
#include "gauss_filter.h"
#include "binarizer.h"
#include "dh_timer.h"

// TODO
#include <QDebug>
#include <ippi.h>
#include <ipps.h>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

#include <iostream>

using namespace cv;
using namespace std;

namespace dh
{
    blob_detector::~blob_detector()
    {
        stop();
    }

    void blob_detector::run( const string& image_path )
    {
        stop();

        _processing_thread = dh_thread( "blob_detector thread",
                                        &blob_detector::processing_thread,
                                        this,
                                        image_path );
    }

    void blob_detector::stop()
    {
        if( _processing_thread.joinable() )
            _processing_thread.join();
    }

    void blob_detector::processing_thread( const string& image_path )
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
        auto filtered_32f = Mat( rows, cols, CV_32FC1 );

        //auto statistics_start_time = dh_timer::now_us();

        image_converter::convert_8u_32f( image_8u, image_32f );

        float const sigma = 2;
        gauss_filter gauss( cols, rows, image_8u.channels(), sigma );

        gauss.filter( image_32f, filtered_32f );

        //
        //dft.forward( image_32f, magnitudes_32f );
        //
        //auto status = ippiLn_32f_C1IR( magnitudes_32f.ptr<float>(),
        //                               static_cast<int>( magnitudes_32f.step ),
        //                               {cols, rows} );
        //if( status != ippStsNoErr )
        //    // throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
        //    qCritical() << ippGetStatusString( status );
        //
        //float min, max;
        //status = ippiMinMax_32f_C1R( magnitudes_32f.ptr<float>(),
        //                             static_cast<int>( magnitudes_32f.step ),
        //                             {cols, rows}, &min, &max );
        //if( status != ippStsNoErr )
        //    qCritical() << ippGetStatusString( status );
        //
        //float sub = min;
        //float div = ( max-min ) / 255.0f;
        //status = ippsNormalize_32f_I( magnitudes_32f.ptr<float>(),
        //                              rows * static_cast<int>( magnitudes_32f.step / sizeof(float) ),
        //                              sub, div );
        //if( status != ippStsNoErr )
        //    qCritical() << ippGetStatusString( status );

        auto blobs_8u = Mat( rows, cols, CV_8UC1 );
        image_converter::convert_32f_8u( filtered_32f, blobs_8u );

        auto blobs_q = QImage( blobs_8u.data, blobs_8u.cols, blobs_8u.rows,
                               static_cast<int>( blobs_8u.step ), QImage::Format_Grayscale8 );

        emit image_processed( blobs_q.copy() );

        // _statistics.period_us = dh_timer::now_us() - statistics_start_time;
        // emit statistics_ready( _statistics );
    }
}
