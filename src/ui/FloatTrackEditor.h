#pragma once

#include <QWidget>

class FloatTrackEditor : public QWidget
{
    Q_OBJECT

public:
    explicit FloatTrackEditor(QWidget *parent = 0);
    ~FloatTrackEditor() {}

private:
    void paintEvent(QPaintEvent * event);

signals:
    void editingFinished();

};
