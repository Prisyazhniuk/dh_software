#include "main_window.h"
#include "fft_processor.h"
#include "blob_detector.h"
#include "image_processing.h"

#include <QApplication>

#include <string>

using namespace dh;
using namespace std;

int main( int argc, char *argv[] )
{	
    try
    {
        QApplication application( argc, argv );

        Q_INIT_RESOURCE( icons );

        image_processing::initialize_ipp();

        fft_processor fft_processor;
        blob_detector blob_detector;

        main_window main_window( &fft_processor, &blob_detector );
        main_window.show();

        qRegisterMetaType<fft_processing_statistics>("fft_processing_statistics");
        qRegisterMetaType<blob_detection_statistics>("blob_detection_statistics");

        QObject::connect( &fft_processor, &fft_processor::image_processed, &main_window, &main_window::image_processed );        
        QObject::connect( &fft_processor, &fft_processor::statistics_ready, &main_window, qOverload<const fft_processing_statistics&>( &main_window::statistics_ready ) );

        QObject::connect( &blob_detector, &blob_detector::image_processed, &main_window, &main_window::image_processed );
        QObject::connect( &blob_detector, &blob_detector::error, &main_window, &main_window::error_notified );
        QObject::connect( &blob_detector, &blob_detector::statistics_ready, &main_window, qOverload<const blob_detection_statistics&>( &main_window::statistics_ready ) );

        return application.exec();
    }
    catch( dh_exception& ex )
    {
        cerr << ex.c_str() << endl;
        exit( EXIT_FAILURE );
    }
    catch( exception& ex )
    {
        cerr << ex.what() << endl;
        exit( EXIT_FAILURE );
    }
    catch( ... )
    {
        cerr << "Unhandled exception in main" << endl;
        exit( EXIT_FAILURE );
    }
}
