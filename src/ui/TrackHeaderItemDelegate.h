#pragma once

#include <QAbstractItemDelegate>

class TrackHeaderItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit TrackHeaderItemDelegate(QObject *parent = 0);
    ~TrackHeaderItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
