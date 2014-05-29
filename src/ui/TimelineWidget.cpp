#include <ui/TimelineWidget.h>
#include <QtGui>

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
    , mOffset(0)
    , mPlaybackPositionInSeconds(8.0)
{

}

TimelineWidget::~TimelineWidget()
{

}

void TimelineWidget::updateOffset(int offset)
{
    mOffset = offset;
    repaint();
}

void TimelineWidget::setPlaybackPosition(double inSeconds)
{
    mPlaybackPositionInSeconds = inSeconds;
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
        int index = (x / 8) + (mOffset / 8);
        if(index % 100 == 0)
        {
            painter.drawLine(x, 0, x, rectHeight);

            QString numberText = QString("%1").arg(index / 100.0);
            int lineoffset = (int)(fontMetrics.width(numberText)/2.0f);
            painter.drawText(x + lineoffset, rectHeight / 2, numberText);
        }
        else if(index % 50 == 0)
        {
            painter.drawLine(x, rectHeight - (lineHeight*2), x, rectHeight);

            QString numberText = QString("%1").arg(index / 100.0);
            int lineoffset = (int)(fontMetrics.width(numberText)/2.0f);
            painter.drawText(x - lineoffset, lineoffset*2, numberText);
        }
        else if(index % 25 == 0)
        {
            painter.drawLine(x, rectHeight - (lineHeight*2), x, rectHeight);
        }
        else
        {
            painter.drawLine(x, rectHeight - lineHeight, x, rectHeight);
        }
    }

    painter.setBrush(Qt::yellow);

    int xPos = (mPlaybackPositionInSeconds * 80) + 1;
    xPos -= mOffset - mOffset % 8;
    QPoint trianglePoints[] =
    {
        { xPos, rectHeight },
        { xPos + 8, rectHeight - lineHeight*2 },
        { xPos - 8, rectHeight - lineHeight*2 }
    };

    painter.drawPolygon(trianglePoints, 3, Qt::WindingFill);
}
