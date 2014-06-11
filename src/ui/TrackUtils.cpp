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

        for(int keyIndex = 0; keyIndex < floatTrack->GetKeyCount(); ++keyIndex)
        {
            const TrackKey<float>& currentKey = floatTrack->GetKey(keyIndex);

            float currentKeyNormalizedValue = NormalizeDataValue(floatTrack->GetMinValue(), floatTrack->GetMaxValue(), currentKey.GetData());

            QPoint currentKeyPos = QPoint(rect.x() + UIConstants::SecondSizeInPixels*currentKey.GetPosition(), rect.y() + rect.height() - ( currentKeyNormalizedValue * rect.height() ));
            keyPoints.push_back(currentKeyPos);
        }

        for(int keyPointIndex = 0; keyPointIndex < keyPoints.size()-1; ++keyPointIndex)
        {
            switch(floatTrack->GetKey(keyPointIndex).GetInterpolationType())
            {
                case kTrackInterpolationType_None:
                {
                    painter->setPen(Qt::black);
                    break;
                }
                case kTrackInterpolationType_Linear:
                {
                    painter->setPen(Qt::red);
                    break;
                }
                case kTrackInterpolationType_Smoothstep:
                {
                    painter->setPen(Qt::green);
                    break;
                }
                case kTrackInterpolationType_Cosine:
                {
                    painter->setPen(Qt::yellow);
                    break;
                }
            }

            painter->drawLine(keyPoints[keyPointIndex], keyPoints[keyPointIndex+1]);
        }

        if(inIsEditMode)
        {
            painter->setPen(Qt::blue);
            for(int keyPointIndex = 0; keyPointIndex < keyPoints.size(); ++keyPointIndex)
            {
                painter->drawEllipse(keyPoints[keyPointIndex], 4, 4);
            }

            painter->setPen(Qt::yellow);
            int tenthSecondSizeInPixels = UIConstants::SecondSizeInPixels / 10;
            int xPos = (inMousePos.x() / tenthSecondSizeInPixels) * tenthSecondSizeInPixels;
            QPoint clampedMousePos = QPoint(xPos, inMousePos.y());
            painter->drawLine(xPos, rect.y(), xPos, rect.y() + rect.height());
            painter->drawEllipse(clampedMousePos, 4, 4);

            for(int keyPointIndex = 0; keyPointIndex < keyPoints.size(); ++keyPointIndex)
            {
                float distance = Distance(clampedMousePos, keyPoints[keyPointIndex]);
                if(distance <= 4.0f)
                {
                    painter->setPen(Qt::yellow);
                    const QFont& currentFont = painter->font();
                    QFontMetrics metrics(currentFont);
                    QString text = QString::number(floatTrack->GetKey(keyPointIndex).GetData());
                    QSize textSize = metrics.size(0, text);
                    QPoint textOffset = QPoint(8, textSize.height() / 4.0f);
                    painter->drawText(clampedMousePos + textOffset, text);

                    break;
                }
            }
        }
    }

    if(inIsEditMode)
    {
        // Draw crosshair for editing points
        painter->setPen(Qt::yellow);
        int tenthSecondSizeInPixels = UIConstants::SecondSizeInPixels / 10;
        int xPos = (inMousePos.x() / tenthSecondSizeInPixels) * tenthSecondSizeInPixels;
        QPoint clampedMousePos = QPoint(xPos, inMousePos.y());
        painter->drawLine(xPos, rect.y(), xPos, rect.y() + rect.height());
        painter->drawEllipse(clampedMousePos, 4, 4);
    }

    painter->restore();
}
