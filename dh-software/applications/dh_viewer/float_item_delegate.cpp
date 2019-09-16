#include "float_item_delegate.h"

#include <QDoubleSpinBox>

namespace dh
{
    float_item_delegate::float_item_delegate( int decimals, double step, QObject* parent )
        : QStyledItemDelegate ( parent )
        , _decimals( decimals )
        , _step( step )
    {}

    QString float_item_delegate::displayText( const QVariant& value, const QLocale& locale ) const
    {
        if( value.type() != QVariant::Double )
            return QStyledItemDelegate::displayText( value, locale );
        else
            return locale.toString( value.toDouble(), 'f' , _decimals );
    }

    QWidget* float_item_delegate::createEditor( QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index) const
    {
        auto editor = QStyledItemDelegate ::createEditor( parent, option, index );

        auto spin_box = qobject_cast<QDoubleSpinBox *>( editor );
        if( spin_box )
        {
            spin_box->setDecimals( _decimals );
            spin_box->setSingleStep( _step );
        }

        return editor;
    }
}
