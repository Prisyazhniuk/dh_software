#include "dft.h"
#include "dh_math.h"
#include "dh_string.h"

#include <ippi.h>
#include <ipps.h>

namespace dh
{
    dft::dft( int width, int height )
        : _width( width )
        , _height( height )
    {
        int context_size;
        int init_buffer_size;
        int work_buffer_size;

        auto status = ippiDFTGetSize_C_32fc( { _width, _height }, _flags, ippAlgHintNone,
                                            &context_size, &init_buffer_size, &work_buffer_size );

        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiDFTGetSize_C_32fc failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }

        _context = reinterpret_cast<IppiDFTSpec_C_32fc*>( ippsMalloc_8u( context_size ) );
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

        status = ippiDFTInit_C_32fc( { _width, _height }, _flags, ippAlgHintNone, _context, _init_buffer );
        if( status != ippStsNoErr )
        {
            auto msg = dh_string::fs( "ippiDFTInit_C_32fc failed: %s", ippGetStatusString( status ) );
            throw image_processing_exception( msg, get_exception_source() );
        }
    }

    dft::~dft()
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
    }

    void dft::forward( const image_32fc& src, image_32fc& dst )
    {
        if( src.width() != _width )
            throw argument_exception( dh_string::fs( "src has wrong width: %d, expected: %d",
                                                     src.width(), _width ),
                                      get_exception_source() );

        if( src.height() != _height )
            throw argument_exception( dh_string::fs( "src has wrong height: %d, expected: %d",
                                                     src.height(), _height ),
                                      get_exception_source() );

        if( dst.width() != _width )
            throw argument_exception( dh_string::fs( "dst has wrong width: %d, expected: %d",
                                                     dst.width(), _width ),
                                      get_exception_source() );

        if( dst.height() != _height )
            throw argument_exception( dh_string::fs( "dst has wrong height: %d, expected: %d",
                                                     dst.height(), _height ),
                                      get_exception_source() );

        auto status = ippiDFTFwd_CToC_32fc_C1R( src.data(), src.step_in_bytes(),
                                                dst.data(), dst.step_in_bytes(),
                                                _context, _work_buffer );
        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }

    void dft::inverse( const image_32fc& src, image_32fc& dst )
    {
        if( src.width() != _width )
            throw argument_exception( dh_string::fs( "src has wrong width: %d, expected: %d",
                                                     src.width(), _width ),
                                      get_exception_source() );

        if( src.height() != _height )
            throw argument_exception( dh_string::fs( "src has wrong height: %d, expected: %d",
                                                     src.height(), _height ),
                                      get_exception_source() );

        if( dst.width() != _width )
            throw argument_exception( dh_string::fs( "dst has wrong width: %d, expected: %d",
                                                     dst.width(), _width ),
                                      get_exception_source() );

        if( dst.height() != _height )
            throw argument_exception( dh_string::fs( "dst has wrong height: %d, expected: %d",
                                                     dst.height(), _height ),
                                      get_exception_source() );

        auto status = ippiDFTInv_CToC_32fc_C1R( src.data(), src.step_in_bytes(),
                                                dst.data(), dst.step_in_bytes(),
                                                _context, _work_buffer );
        if( status != ippStsNoErr )
            throw image_processing_exception( ippGetStatusString( status ), get_exception_source() );
    }
}
