#pragma once

#include <QStyledItemDelegate>

class FloatTrack;

class TrackItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT

public:
    explicit TrackItemDelegate(QObject *parent = 0);
    ~TrackItemDelegate();

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;

    QWidget* createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;

private slots:
    void commitAndCloseEditor();
};
