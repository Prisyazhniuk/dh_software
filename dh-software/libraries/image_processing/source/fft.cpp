#include "fft.h"
#include "dh_math.h"
#include "dh_string.h"

#include <ippi.h>
#include <ipps.h>

using namespace cv;

fft::fft( int width, int height, int channels )
{
    uint32_t order_width;
    uint32_t order_height;

    if( channels != 1 )
        throw argument_exception( "only one-channel images supported", get_exception_source() );

    auto x_order = dh_math::power_of_2( uint32_t( width ), order_width );
    auto y_order = dh_math::power_of_2( uint32_t( height ), order_height );

    _width = int( order_width );
    _height = int( order_height );

    int context_size;
    int init_buffer_size;
    int work_buffer_size;

    auto status = ippiFFTGetSize_R_32f( x_order, y_order, _flags, _hint,
                                        &context_size, &init_buffer_size, &work_buffer_size );

    if( status != ippStsNoErr )
    {
        auto msg = dh_string::fs( "ippiFFTGetSize_R_32f failed: %s", ippGetStatusString( status ) );
        throw image_processing_exception( msg, get_exception_source() );
    }

    _context = reinterpret_cast<IppiFFTSpec_R_32f*>( ippsMalloc_8u( context_size ) );
    if( !_context )
    {
        auto msg = dh_string::fs( "ippsMalloc_8u for _context failed (%d bytes)", context_size );
        throw image_processing_exception( msg, get_exception_source() );
    }

    if( init_buffer_size )
    {
        _init_buffer = ippsMalloc_8u( init_buffer_size );
        if( !_init_buffer )
        {
            auto msg = dh_string::fs( "ippsMalloc_8u for _init_buffer failed (%d bytes)", init_buffer_size );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }
    else
    {
        _init_buffer = nullptr;
    }

    _work_buffer = ippsMalloc_8u( work_buffer_size );
    if( !_work_buffer )
    {
        auto msg = dh_string::fs( "ippsMalloc_8u for _work_buffer failed (%d bytes)", work_buffer_size );
        throw image_processing_exception( msg, get_exception_source() );
    }

    status = ippiFFTInit_R_32f( x_order, y_order, _flags, _hint, _context, _init_buffer );
    if( status != ippStsNoErr )
    {
        auto msg = dh_string::fs( "ippiFFTInit_R_32f failed: %s", ippGetStatusString( status ) );
        throw image_processing_exception( msg, get_exception_source() );
    }

    int repack_buffer_size;
    status = ippiMagnitudePackGetBufferSize_32f( channels, { _width, _height }, &repack_buffer_size );
    if( status != ippStsNoErr )
    {
        auto msg = dh_string::fs( "ippiPhasePackGetBufferSize_32f failed: %s", ippGetStatusString( status ) );
        throw image_processing_exception( msg, get_exception_source() );
    }

    _repack_buffer = ippsMalloc_8u( repack_buffer_size );
    if( !_repack_buffer )
    {
        auto msg = dh_string::fs( "ippsMalloc_8u for _repack_buffer failed (%d bytes)", repack_buffer_size );
        throw image_processing_exception( msg, get_exception_source() );
    }

    _coefficients = Mat( _height, _width, CV_32FC1 );
}

fft::~fft()
{
    if( _context )
    {
        ippsFree( _context );
        _context = nullptr;
    }

    if( _init_buffer )
    {
        ippsFree( _init_buffer );
        _init_buffer = nullptr;
    }

    if( _work_buffer )
    {
        ippsFree( _work_buffer );
        _work_buffer = nullptr;
    }

    if( _repack_buffer )
    {
        ippsFree( _repack_buffer );
        _repack_buffer = nullptr;
    }
}

int fft::get_width()
{
    return _width;
}

int fft::get_height()
{
    return _height;
}

void fft::forward( const Mat& src, Mat& magnitudes )
{
    if( src.cols != _width )
        throw argument_exception( dh_string::fs( "src has wrong width: %d, expected: %d",
                                                 src.cols, _width ),
                                  get_exception_source() );

    if( src.rows != _height )
        throw argument_exception( dh_string::fs( "src has wrong height: %d, expected: %d",
                                                 src.rows, _height ),
                                  get_exception_source() );

    if( src.channels() != 1 )
        throw argument_exception( dh_string::fs( "src has wrong channels count: %d, expected: %d",
                                                 src.channels(), 1 ),
                                  get_exception_source() );

    if( src.depth() != CV_32F )
        throw argument_exception( "src must be CV_32F image", get_exception_source() );

    if( magnitudes.cols != _width )
        throw argument_exception( dh_string::fs( "magnitudes has wrong width: %d, expected: %d",
                                                 magnitudes.cols, _width ),
                                  get_exception_source() );

    if( magnitudes.rows != _height )
        throw argument_exception( dh_string::fs( "magnitudes has wrong height: %d, expected: %d",
                                                 magnitudes.rows, _height ),
                                  get_exception_source() );

    if( magnitudes.channels() != 1 )
        throw argument_exception( dh_string::fs( "magnitudes has wrong channels count: %d, expected: %d",
                                                 magnitudes.channels(), 1 ),
                                  get_exception_source() );

    if( magnitudes.depth() != CV_32F )
        throw argument_exception( "magnitudes must be CV_32F image", get_exception_source() );

    auto status = ippiFFTFwd_RToPack_32f_C1R( src.ptr<float>(), static_cast<int>( src.step ),
                                              _coefficients.ptr<float>(), static_cast<int>( _coefficients.step ),
                                              _context, _work_buffer );
    if( status != ippStsNoErr )
        throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );


    status = ippiMagnitudePack_32f_C1R( _coefficients.ptr<float>(), static_cast<int>( _coefficients.step ),
                                        magnitudes.ptr<float>(), static_cast<int>( magnitudes.step ),
                                        { _coefficients.cols, _coefficients.rows },
                                        _repack_buffer );
    if( status != ippStsNoErr )
        throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
}
