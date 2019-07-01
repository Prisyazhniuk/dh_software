#include "error_reporter.h"

namespace dh
{
    error_reporter::error_reporter( QObject *parent )
        : QObject(parent)
    {}

    error_reporter::~error_reporter()
    {}
}
