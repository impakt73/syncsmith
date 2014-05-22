#include <TimelineWidget.h>
#include <QtGui>

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
{

}

TimelineWidget::~TimelineWidget()
{

}

void TimelineWidget::paintEvent(QPaintEvent *event)
{
    const QRect& rect = this->rect();

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    int rectHeight = rect.height();
    int lineHeight = rectHeight / 4;

    QFont font("Arial", 10);
    QFontMetricsF fontMetrics(font);

    painter.setPen(Qt::black);
    painter.setFont(font);
    for(int x = 1; x < rect.width()-1; x += 8)
    {
        int index = x / 8;
        if(index % 10 == 0)
        {
            painter.drawLine(x, 0, x, rectHeight);

            QString numberText = QString("%1").arg(index / 10.0);
            int lineoffset = (int)(fontMetrics.width(numberText)/2.0f);
            painter.drawText(x + lineoffset, rectHeight / 2, numberText);
        }
        else if(index % 5 == 0)
        {
            painter.drawLine(x, rectHeight - (lineHeight*2), x, rectHeight);
        }
        else
        {
            painter.drawLine(x, rectHeight - lineHeight, x, rectHeight);
        }
    }
}
