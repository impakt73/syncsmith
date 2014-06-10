#include <ui/TrackUtils.h>
#include <core/Track.h>
#include <ui/UIConstants.h>
#include <core/FloatTrack.h>

void TrackUtils::RenderTrackBackground(QPainter *painter, const QRect &rect, const Track *inTrack)
{
    painter->save();
    //painter->setPen(Qt::NoPen);
    //painter->fillRect(rect, Qt::gray);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    pen.setBrush(Qt::black);
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);

    int xCenter = rect.x() + rect.width() / 2;
    QLinearGradient gradient(xCenter, rect.y(), xCenter, rect.y() + rect.height());
    gradient.setInterpolationMode(QLinearGradient::ColorInterpolation);
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::darkGray);
    painter->setBrush(gradient);

    painter->drawRect(rect.x() + 3, rect.y() + 3, rect.width() - 4, rect.height() - 4);

    painter->restore();
}

void TrackUtils::RenderFloatTrack(QPainter *painter, const QRect &rect, const Track *inTrack, bool inIsEditMode, const QPoint& inMousePos)
{
    Q_ASSERT(inTrack->GetType() == kTrackType_Float);
    const FloatTrack* floatTrack = static_cast<const FloatTrack*>(inTrack);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing, true);

    if(floatTrack->GetKeyCount() > 0)
    {
        painter->setPen(Qt::black);

        QVector<QPoint> keyPoints;
        keyPoints.reserve(floatTrack->GetKeyCount());

        const TrackKey<float>& firstKey = floatTrack->GetKey(0);
        float trackNormalizedValue = NormalizeDataValue(0.0f, 100.0f, firstKey.GetData());
        QPoint firstKeyPos = QPoint((rect.x() + 3) + UIConstants::SecondSizeInPixels*firstKey.GetPosition(), (rect.y() + 3) + (rect.height() - 4) - ( trackNormalizedValue * (rect.height() - 4) ));

        keyPoints.push_back(firstKeyPos);

        for(int keyIndex = 0; keyIndex < floatTrack->GetKeyCount(); ++keyIndex)
        {
            const TrackKey<float>& currentKey = floatTrack->GetKey(keyIndex);

            float currentKeyNormalizedValue = NormalizeDataValue(0.0f, 100.0f, currentKey.GetData());

            QPoint currentKeyPos = QPoint((rect.x() + 3) + UIConstants::SecondSizeInPixels*currentKey.GetPosition(), (rect.y() + 3) + (rect.height() - 4) - ( currentKeyNormalizedValue * (rect.height() - 4) ));
            keyPoints.push_back(currentKeyPos);
        }

        for(int keyPointIndex = 0; keyPointIndex < keyPoints.size()-1; ++keyPointIndex)
        {
            painter->drawLine(keyPoints[keyPointIndex], keyPoints[keyPointIndex+1]);
        }

        if(inIsEditMode)
        {
            painter->setPen(Qt::green);
            for(int keyPointIndex = 0; keyPointIndex < keyPoints.size(); ++keyPointIndex)
            {
                float distance = Distance(inMousePos, keyPoints[keyPointIndex]);
                if(distance <= 4.0f)
                {
                    painter->setPen(Qt::red);
                    painter->drawEllipse(keyPoints[keyPointIndex], 4, 4);
                    painter->setPen(Qt::green);
                }
                else
                {
                    painter->drawEllipse(keyPoints[keyPointIndex], 4, 4);
                }
            }
        }
    }

    painter->restore();
}
