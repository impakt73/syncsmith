#include <TrackItemDelegate.h>
#include <QtGui>

TrackItemDelegate::TrackItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

TrackItemDelegate::~TrackItemDelegate()
{

}

void TrackItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->fillRect(option.rect, Qt::gray);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    if(option.state & QStyle::State_Selected)
    {
        pen.setBrush(Qt::red);
    }
    else
    {
        pen.setBrush(Qt::black);
    }
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::gray);
    painter->drawRect(option.rect.x() + 3, option.rect.y() + 3, option.rect.width() - 4, option.rect.height() - 4);

    painter->drawText(QPoint(option.rect.x() + option.rect.width() / 2, option.rect.y() + option.rect.height() / 2), qvariant_cast<QString>(index.data()));
    painter->restore();
}

QSize TrackItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(1000, 50);
}
