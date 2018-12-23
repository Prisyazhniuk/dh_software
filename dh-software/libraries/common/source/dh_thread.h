#pragma once

#include "dh_exception.h"

#include <string>
#include <thread>
#include <functional>
#include <exception>
#include <iostream>

class dh_thread
{
public:
	dh_thread();
	dh_thread( dh_thread&& );
	dh_thread( dh_thread& ) = delete;
	dh_thread( const dh_thread& ) = delete;
	dh_thread( const dh_thread&& ) = delete;

	dh_thread& operator=( const dh_thread& ) = delete;
	dh_thread& operator=( dh_thread&& );

	template<class F, class... Args>
	dh_thread( const std::string&, F&&, Args&&... );

	void join();
	bool joinable();

private:
	std::thread _thread;
};

template<class F, class... Args>
dh_thread::dh_thread( const std::string& name, F&& f, Args&&... args )
{
	auto f_args = std::bind( f, args... );

	_thread = std::thread( [name, f_args ]()
	{
		try
		{
			f_args();
		}
		catch( dh_exception& ex )
		{
			std::cerr << "Exception in " << name << std::endl;
			std::cerr << ex.c_str() << std::endl;
			exit( EXIT_FAILURE );
		}
		catch( std::exception& ex )
		{
			std::cerr << "Exception in " << name << std::endl;
			std::cerr << ex.what() << std::endl;
			exit( EXIT_FAILURE );
		}
		catch( ... )
		{
			std::cerr << "Unhandled exception in " << name << std::endl;
			exit( EXIT_FAILURE );
		}
	} );
}
