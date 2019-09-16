#pragma once

#include <QStyledItemDelegate >

namespace dh
{
    class float_item_delegate: public QStyledItemDelegate
    {
    public:
        float_item_delegate( int decimals, double step, QObject *parent = nullptr );

        virtual QString displayText( const QVariant& value, const QLocale& locale ) const override;
        virtual QWidget* createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const override;

    private:
        int _decimals;
        double _step;
    };
}
