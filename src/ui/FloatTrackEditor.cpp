#include <ui/FloatTrackEditor.h>
#include <ui/TrackUtils.h>
#include <core/Track.h>


FloatTrackEditor::FloatTrackEditor(Track* inTrack, QWidget *parent)
    : QWidget(parent)
    , mTrack(inTrack)
    , mLastMousePosition(0,0)
{
    setAutoFillBackground(true);
    setMouseTracking(true);
}

void FloatTrackEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    TrackUtils::RenderTrackBackground(&painter, this->rect(), mTrack);
    TrackUtils::RenderFloatTrack(&painter, this->rect(), mTrack, true, mLastMousePosition);
}

void FloatTrackEditor::mouseMoveEvent(QMouseEvent *event)
{
    mLastMousePosition = event->pos();
    update();
}

void FloatTrackEditor::mousePressEvent(QMouseEvent *event)
{
    mLastMousePosition = event->pos();
}

void FloatTrackEditor::mouseReleaseEvent(QMouseEvent *event)
{
    mLastMousePosition = event->pos();
}
