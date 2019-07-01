#pragma once

#include "blob_detection_statistics.h"

#include <QAbstractTableModel>
#include <atomic>

namespace dh
{
    class blob_detection_statistics_model : public QAbstractTableModel
    {
    public:
        blob_detection_statistics_model( QObject* parent = nullptr );

        virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
        virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

        virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    public slots:
        void update_statistics( const blob_detection_statistics& );

    private:
        static const int _rows = 3;
        static const int _cols = 2;

        std::atomic<blob_detection_statistics> _statistics;
    };
}
