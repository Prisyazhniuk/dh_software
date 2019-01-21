#include "image_processing.h"

#include <ippcore.h>

void image_processing::initialize_ipp()
{
	auto status = ippInit();

	if( status != ippStsNoErr )
		throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
}
