#include "main_window.h"
#include "fft_viewer_core.h"

#include <QApplication>

#include <string>

using namespace std;

int main( int argc, char *argv[] )
{
	try
	{
		QApplication application( argc, argv );

		fft_viewer_core core;
		main_window main_window( &core );

		main_window.show();

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
