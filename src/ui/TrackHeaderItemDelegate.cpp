#include <ui/TrackHeaderItemDelegate.h>
#include <QtGui>

TrackHeaderItemDelegate::TrackHeaderItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

TrackHeaderItemDelegate::~TrackHeaderItemDelegate()
{

}

void TrackHeaderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
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

    int xCenter = option.rect.x() + option.rect.width() / 2;
    QLinearGradient gradient(xCenter, option.rect.y(), xCenter, option.rect.y() + option.rect.height());
    gradient.setInterpolationMode(QLinearGradient::ColorInterpolation);
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::darkGray);
    painter->setBrush(gradient);

    painter->drawRect(option.rect.x() + 3, option.rect.y() + 3, option.rect.width() - 4, option.rect.height() - 4);

    QFont font("Arial", 10);
    QFontMetricsF fontMetrics(font);
    QString text = qvariant_cast<QString>(index.data());
    QSizeF fontSize = fontMetrics.size(0, text);

    painter->setPen(Qt::black);
    painter->setFont(font);

    painter->drawText(QPoint(option.rect.x() + ( option.rect.width() / 2 - fontSize.width() / 2 ), option.rect.y() + option.rect.height() / 2), text);
    painter->restore();
}

QSize TrackHeaderItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 100);
}
