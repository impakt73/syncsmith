#include <ui/TimelineWidget.h>
#include <QtGui>
#include <ui/UIConstants.h>

TimelineWidget::TimelineWidget(QWidget *parent)
    : QWidget(parent)
    , mScrollOffset(0.0)
    , mPlaybackPositionInSeconds(8.0)
    , mPlaybackPositionOffset(0.0)
    , mLengthInSeconds(0.0)
{
    //setAttribute(Qt::WA_OpaquePaintEvent);
}

TimelineWidget::~TimelineWidget()
{

}

void TimelineWidget::updateScrollOffset(double inScrollOffset)
{
    mScrollOffset = inScrollOffset;
    repaint();
}

void TimelineWidget::setPlaybackPosition(double inSeconds)
{
    mPlaybackPositionInSeconds = inSeconds;
    repaint();
}

void TimelineWidget::setLength(double inSeconds)
{
    mLengthInSeconds = inSeconds;
    repaint();
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
    static const int sTickStepInPixels = UIConstants::SecondSizeInPixels/10;
    int xLeft = (mScrollOffset * UIConstants::SecondSizeInPixels) - 1; // Subtract 1 for anti-aliasing
    painter.translate(-xLeft, 0.0);

    for(int x = 0; x < mLengthInSeconds*UIConstants::SecondSizeInPixels; x += sTickStepInPixels)
    {
        int index = (x / sTickStepInPixels);
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

    painter.setBrush(Qt::yellow);

    int xPos = (mPlaybackPositionInSeconds * UIConstants::SecondSizeInPixels);
    int triangleHeight = (lineHeight * 1.5f);
    QPoint trianglePoints[] =
    {
        { xPos, rectHeight },
        { xPos + 5, rectHeight - triangleHeight },
        { xPos - 5, rectHeight - triangleHeight }
    };

    painter.drawPolygon(trianglePoints, 3, Qt::WindingFill);
/*
    for(int x = 1; x < rect.width()-1; x += 8)
    {
        int index = (x / 8.0f) + (mOffset / 8.0f);
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
    */
}
