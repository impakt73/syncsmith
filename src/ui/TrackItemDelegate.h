#pragma once

#include <QAbstractItemDelegate>

class FloatTrack;

class TrackItemDelegate : public QAbstractItemDelegate
{
    Q_OBJECT

public:
    explicit TrackItemDelegate(QObject *parent = 0);
    ~TrackItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

private:
    void RenderFloatTrack(QPainter *painter, const QStyleOptionViewItem &option, const FloatTrack* inFloatTrack) const;
};
