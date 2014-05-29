#pragma once

#include <QAbstractItemDelegate>

class TrackItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit TrackItemDelegate(QObject *parent = 0);
    ~TrackItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};
