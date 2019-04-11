#include "blob_detector.h"
#include "image_converter.h"
#include "gauss_filter.h"
#include "binarizer.h"
#include "label_marker.h"
#include "blob_calculator.h"

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
        auto filtered_8u = Mat( rows, cols, CV_8UC1 );
        auto blobs_8u = Mat( rows, cols, CV_8UC1 );

        //auto statistics_start_time = dh_timer::now_us();

        image_converter::convert_8u_32f( image_8u, image_32f );

        float const sigma = 2;
        gauss_filter gauss( cols, rows, image_8u.channels(), sigma );

        gauss.filter( image_32f, filtered_32f );

        image_converter::convert_32f_8u( filtered_32f, filtered_8u );

        const float binarization_threshold = 0.90f;
        binarizer::binarize( filtered_8u, blobs_8u, binarization_threshold );

        label_marker marker( cols, rows, blobs_8u.channels() );

        auto blobs_count = marker.mark( blobs_8u );

        auto blobs = blob_calculator::calculate( blobs_8u, blobs_count );

        blobs_8u = blobs_8u != 0;
        auto blobs_8u_c3 = Mat( rows, cols, CV_8UC3 );
        cvtColor( blobs_8u, blobs_8u_c3, COLOR_GRAY2RGB );
        auto blobs_q = QImage( blobs_8u_c3.data, blobs_8u_c3.cols, blobs_8u_c3.rows,
                               static_cast<int>( blobs_8u_c3.step ), QImage::Format_RGB888 );

        //if( blobs_count < 3 )
        //{
        //
        //}
        //else
        //{
        //    blob b1, b2;
        //    find_two_largest_blobs( move(blobs), blobs_count, b1, b2 );
        //}

        emit image_processed( blobs_q.copy() );

        // _statistics.period_us = dh_timer::now_us() - statistics_start_time;
        // emit statistics_ready( _statistics );
    }

    void blob_detector::find_two_largest_blobs( shared_ptr<blob[]> blobs, size_t blobs_count, blob& b1, blob& b2 )
    {

    }
}
