#pragma once

#include "processing_statistics.h"

#include <QAbstractTableModel>
#include <atomic>

namespace dh
{
    class processing_statistics_model: public QAbstractTableModel
    {
    public:
        processing_statistics_model( QObject* parent = nullptr );

        virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
        virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

        virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    public slots:
        void update_statistics( const processing_statistics& );

    private:
        static const int _rows = 1;
        static const int _cols = 2;

        std::atomic<processing_statistics> _statistics;
    };
}
