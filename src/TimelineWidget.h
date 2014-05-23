#pragma once

#include <QWidget>

class TimelineWidget : public QWidget
{
    Q_OBJECT

public:
    explicit TimelineWidget(QWidget *parent = 0);
    ~TimelineWidget();

    inline int offset() const { return mOffset; }

public slots:
    void updateOffset(int offset);

private:
    void paintEvent(QPaintEvent * event);
    int mOffset;
};
