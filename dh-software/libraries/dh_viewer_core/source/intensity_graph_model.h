#pragma once

#include "draggable_cursor.h"

#include "exceptions/argument_exception.h"

#include <QAbstractTableModel>
#include <QGraphicsScene>

namespace dh
{
    class intensity_graph_model : public QAbstractTableModel
    {
        Q_OBJECT

    public:
        intensity_graph_model( QGraphicsScene*, QGraphicsPixmapItem*, QObject* parent = nullptr );

        virtual int rowCount( const QModelIndex& parent = QModelIndex() ) const override;
        virtual int columnCount( const QModelIndex& parent = QModelIndex() ) const override;

        virtual QVariant data( const QModelIndex& index, int role = Qt::DisplayRole ) const override;
        virtual bool setData( const QModelIndex& index, const QVariant& value, int role = Qt::EditRole ) override;

        virtual Qt::ItemFlags flags( const QModelIndex& index ) const override;

    public slots:
        void image_processed( const QImage& );

    private slots:
        void cursor_moved( const QPointF& );

    private:
        void enable();
        void disable();

        QPointF toPoint( const QString& ) const ;
        QString toString( const QPointF& ) const;

    private:
        bool _first_enable;

        QGraphicsScene* _scene;
        QGraphicsPixmapItem* _scene_item;

        bool _enabled;
        draggable_cursor* _cursor_1;
        draggable_cursor* _cursor_2;

    private:
        static const int _rows = 3;
        static const int _cols = 2;
    };
}
