#pragma once

#include <QObject>

namespace dh
{
    class error_reporter : public QObject
    {
        Q_OBJECT

    public:
        explicit error_reporter( QObject *parent = nullptr );
        virtual ~error_reporter();

    signals:
        void error( const QString& message );
    };
}
