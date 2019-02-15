#include "main_window.h"
#include "fft_viewer_core.h"
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

		fft_viewer_core core;
        main_window main_window( &core );
        main_window.show();

        QObject::connect( &core, &fft_viewer_core::image_processed,
                          &main_window, &main_window::show_image );


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
