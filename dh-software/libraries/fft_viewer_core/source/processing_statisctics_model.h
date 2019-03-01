#pragma once

#include "processing_statistics.h"

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
        void update_statistics( const processing_statistics& );

    private:
        double fps() const;

    private:
        static const int _rows = 1;
        static const int _cols = 2;

        processing_statistics _statistics;
    };
}
