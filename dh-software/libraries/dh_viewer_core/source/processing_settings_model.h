#pragma once

#include "processing_settings.h"

#include <QAbstractTableModel>
#include <atomic>

namespace dh
{
    class processing_settings_model: public QAbstractTableModel
    {
        Q_OBJECT

    public:
        processing_settings_model( const processing_settings&, QObject* parent = nullptr );

        processing_settings get();

        virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
        virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

        virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
        virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

        virtual Qt::ItemFlags flags( const QModelIndex& index ) const override;

    private:
        static const int _rows = 5;
        static const int _cols = 2;

        std::atomic<processing_settings> _settings;
    };
}
