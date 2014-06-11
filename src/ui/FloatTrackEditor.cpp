#include <ui/FloatTrackEditor.h>
#include <ui/TrackUtils.h>
#include <core/Track.h>
#include <ui/UIConstants.h>
#include <core/FloatTrack.h>

FloatTrackEditor::FloatTrackEditor(Track* inTrack, QWidget *parent)
    : QWidget(parent)
    , mTrack(inTrack)
    , mLastMousePosition(0,0)
    , mIsEditingKey(false)
{
    setMouseTracking(true);
    setCursor(Qt::BlankCursor);
    setAttribute(Qt::WA_OpaquePaintEvent);
}

void FloatTrackEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    TrackUtils::RenderTrackBackground(&painter, this->rect(), mTrack);
    TrackUtils::RenderFloatTrack(&painter, this->rect(), mTrack, true, mLastMousePosition);
}

inline float clamp(float x, float a, float b)
{
    return x < a ? a : (x > b ? b : x);
}

void FloatTrackEditor::mouseMoveEvent(QMouseEvent *event)
{
    if(mIsEditingKey)
    {
        int yValue = clamp(event->pos().y(), rect().y(), rect().y() + rect().height());
        mLastMousePosition = QPoint(mLastMousePosition.x(), yValue);
        QCursor::setPos(mapToGlobal(mLastMousePosition));

        // Cut the position down into increments of tenths of a second
        double keyPosition = (mLastMousePosition.x() / (UIConstants::SecondSizeInPixels / 10)) / 10.0f;
        float dataValue = (1.0f - ((float)(mLastMousePosition.y() - rect().y()) / (float)rect().height())) * 100.0f;
        FloatTrack* floatTrack = static_cast<FloatTrack*>(mTrack);
        unsigned int keyIndex = 0;
        if(floatTrack->GetKeyIndex(keyPosition, &keyIndex))
        {
            floatTrack->GetKey(keyIndex).SetData(dataValue);
        }
    }
    else
    {
        mLastMousePosition = event->pos();
    }

    update();
}

void FloatTrackEditor::mousePressEvent(QMouseEvent *event)
{
    mLastMousePosition = event->pos();
    mIsEditingKey = true;
    update();
}

void FloatTrackEditor::mouseReleaseEvent(QMouseEvent *event)
{
    mLastMousePosition = event->pos();

    mIsEditingKey = false;
    update();
}
