#include <ui/FloatTrackEditor.h>
#include <ui/TrackUtils.h>
#include <core/Track.h>


FloatTrackEditor::FloatTrackEditor(Track* inTrack, QWidget *parent)
    : QWidget(parent)
    , mTrack(inTrack)
{
    setAutoFillBackground(true);
}

void FloatTrackEditor::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, true);
    TrackUtils::RenderTrackBackground(&painter, this->rect(), mTrack);
    TrackUtils::RenderFloatTrack(&painter, this->rect(), mTrack, true);
}

void FloatTrackEditor::mousePressEvent(QMouseEvent *event)
{

}
