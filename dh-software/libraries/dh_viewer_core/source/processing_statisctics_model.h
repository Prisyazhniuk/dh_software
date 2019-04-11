#pragma once

#include "fft_processing_statistics.h"
#include "blob_detection_statistics.h"

#include <QAbstractTableModel>

namespace dh
{
    class processing_statisctics_model : public QAbstractTableModel
    {
    public:
        processing_statisctics_model( QObject* parent = nullptr );

        virtual int rowCount( const QModelIndex &parent = QModelIndex() ) const override;
        virtual int columnCount( const QModelIndex &parent = QModelIndex() ) const override;

        virtual QVariant data( const QModelIndex &index, int role = Qt::DisplayRole ) const override;

    public slots:
        void update_statistics( const fft_processing_statistics& );
        void update_statistics( const blob_detection_statistics& );

    private:
        double fps() const;

    private:
        static const int _rows = 3; // 1;
        static const int _cols = 2;

        fft_processing_statistics _fft_processing_statistics;
        blob_detection_statistics _blob_detection_statistics;
    };
}
