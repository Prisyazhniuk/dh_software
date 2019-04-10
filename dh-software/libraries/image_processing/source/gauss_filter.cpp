#include "gauss_filter.h"
#include "dh_string.h"

#include <ippcore.h>
#include <ippvm.h>
#include <ippcv.h>

using namespace cv;

namespace dh
{
    gauss_filter::gauss_filter( int width, int height, int channels, float sigma )
    {
        if( channels != 1 )
            throw argument_exception( "only one-channel images supported", get_exception_source() );

        if( sigma <= 0 )
            throw argument_exception( "sigma must be greater than zero", get_exception_source() );

        Ipp32f twice_sigma = 2 * sigma;
        Ipp32f ceil_twice_sigma = 0;
        auto status = ippsCeil_32f( &twice_sigma, &ceil_twice_sigma, 1 );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippsCeil_32f failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        uint32_t kernel_size = 2 * uint32_t(ceil_twice_sigma) + 1;

        int context_size = 0;
        int buffer_size = 0;
        status = ippiFilterGaussianGetBufferSize( { width, height }, kernel_size, ipp32f, channels,
                                                  &context_size, &buffer_size );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiFilterGaussianGetBufferSize failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        _context = reinterpret_cast<IppFilterGaussianSpec*>( ippMalloc( context_size ) );
        if( !_context )
        {
            auto msg = dh_string::fs( "ippMalloc for _context failed (%d bytes)", context_size );
            throw image_processing_exception( msg, get_exception_source() );
        }

        _buffer = reinterpret_cast<uint8_t*>( ippMalloc( buffer_size ) );
        if( !_buffer )
        {
            auto msg = dh_string::fs( "ippMalloc for _buffer failed (%d bytes)", buffer_size );
            throw image_processing_exception( msg, get_exception_source() );
        }

        status = ippiFilterGaussianInit( { width, height }, kernel_size, sigma,
                                         ippBorderConst, ipp32f, channels,
                                         _context, _buffer );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiFilterGaussianInit failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }

    gauss_filter::~gauss_filter()
    {
        if( _context )
        {
            ippFree( _context );
            _context = nullptr;
        }

        if( _buffer )
        {
            ippFree( _buffer );
            _buffer = nullptr;
        }
    }

    void gauss_filter::filter( const cv::Mat& src, cv::Mat& dst )
    {
        // TODO
        // some checks like fft

        // use _cols and _rows

        auto status = ippiFilterGaussianBorder_32f_C1R( src.ptr<float>(), static_cast<int>( src.step ),
                                                        dst.ptr<float>(), static_cast<int>( dst.step ),
                                                        { src.cols, src.rows }, 0.0f,
                                                        _context, _buffer );
        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }
}
