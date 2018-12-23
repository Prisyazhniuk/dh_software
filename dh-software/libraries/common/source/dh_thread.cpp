#include "dh_thread.h"

dh_thread::dh_thread()
{}

dh_thread::dh_thread( dh_thread&& other )
{
	_thread = std::move( other._thread );
}

void dh_thread::join()
{
	_thread.join();
}

bool dh_thread::joinable()
{
	return _thread.joinable();
}

dh_thread& dh_thread::operator=( dh_thread&& other )
{
	_thread = std::move( other._thread );
	return *this;
}
