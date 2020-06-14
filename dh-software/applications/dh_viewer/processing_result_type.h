#pragma once

#include <QMetaType>

namespace dh
{
    enum class processing_result_type
    {
        original,
        phase,
        amplitude,
        intensity,
        real,
        imaginary
    };
}

Q_DECLARE_METATYPE( dh::processing_result_type );
