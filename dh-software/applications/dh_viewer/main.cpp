#include "main_window.h"
#include "hologram_processor.h"
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

        hologram_processor hologram_processor;

        main_window main_window( &hologram_processor );
        main_window.show();

        qRegisterMetaType<processing_results>("processing_results");
        qRegisterMetaType<processing_statistics>("processing_statistics");

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
