#include <ui/TrackItemDelegate.h>
#include <QtGui>
#include <ui/TrackListModel.h>
#include <ui/UIConstants.h>
#include <core/FloatTrack.h>

TrackItemDelegate::TrackItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

TrackItemDelegate::~TrackItemDelegate()
{

}

void TrackItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    model->
    /*
    const std::vector<unsigned short>* audioSamples = model->getAudioSamples();

    painter->save();
    painter->setPen(Qt::NoPen);
    painter->fillRect(option.rect, Qt::gray);
    painter->setRenderHint(QPainter::Antialiasing, true);
    QPen pen;
    if(option.state & QStyle::State_Selected)
    {
        pen.setBrush(Qt::red);
    }
    else
    {
        pen.setBrush(Qt::black);
    }
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::gray);

    int xCenter = option.rect.x() + option.rect.width() / 2;
    QLinearGradient gradient(xCenter, option.rect.y(), xCenter, option.rect.y() + option.rect.height());
    gradient.setInterpolationMode(QLinearGradient::ColorInterpolation);
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::darkGray);
    painter->setBrush(gradient);

    painter->drawRect(option.rect.x() + 3, option.rect.y() + 3, option.rect.width() - 4, option.rect.height() - 4);

    if(audioSamples != nullptr)
    {
        for(int sampleIndex = 0; sampleIndex < audioSamples->size() && sampleIndex*3 < option.rect.width(); ++sampleIndex)
        {
            unsigned short sampleValue = (*audioSamples)[sampleIndex];
            float barLength = (float)(sampleValue - model->getMinSample()) / (float)(model->getMaxSample() - model->getMinSample());
            int barLengthInPixels = barLength * ((option.rect.height() / 2.0f) - 4);
            painter->drawLine(option.rect.x() + sampleIndex*8, (option.rect.height()/2) - barLengthInPixels, option.rect.x() + sampleIndex*8, (option.rect.height()/2) + barLengthInPixels);
        }
    }

    painter->drawText(QPoint(option.rect.x() + option.rect.width() / 2, option.rect.y() + option.rect.height() / 2), qvariant_cast<QString>(index.data()));
    painter->restore();
    */
}

QSize TrackItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    return QSize(model->getAudioLength() * UIConstants::SecondSizeInPixels, 100);
}

float NormalizeDataValue(float inMinDataValue, float inMaxDataValue, float inDataValue)
{
    return (inDataValue - inMinDataValue) / (inMaxDataValue - inMinDataValue);
}

void TrackItemDelegate::RenderFloatTrack(QPainter *painter, const QStyleOptionViewItem &option, FloatTrack *inFloatTrack)
{
    painter->save();
    painter->setPen(Qt::NoPen);
    painter->fillRect(option.rect, Qt::gray);
    painter->setRenderHint(QPainter::Antialiasing, true);

    QPen pen;
    if(option.state & QStyle::State_Selected)
    {
        pen.setBrush(Qt::red);
    }
    else
    {
        pen.setBrush(Qt::black);
    }
    pen.setWidth(1);
    pen.setCapStyle(Qt::RoundCap);
    pen.setJoinStyle(Qt::RoundJoin);
    painter->setPen(pen);
    painter->setBrush(Qt::gray);

    int xCenter = option.rect.x() + option.rect.width() / 2;
    QLinearGradient gradient(xCenter, option.rect.y(), xCenter, option.rect.y() + option.rect.height());
    gradient.setInterpolationMode(QLinearGradient::ColorInterpolation);
    gradient.setColorAt(0, Qt::lightGray);
    gradient.setColorAt(1, Qt::darkGray);
    painter->setBrush(gradient);

    painter->drawRect(option.rect.x() + 3, option.rect.y() + 3, option.rect.width() - 4, option.rect.height() - 4);

    if(inFloatTrack->GetKeyCount() > 0)
    {
        pen.setBrush(Qt::green);
        painter->setBrush(Qt::green);

        QPainterPath painterPath;

        TrackKey<float>& firstKey = inFloatTrack->GetKey(0);
        float trackNormalizedValue = NormalizeDataValue(0.0f, 100.0f, firstKey.GetData());
        QPoint firstKeyPos = QPoint(UIConstants::SecondSizeInPixels*firstKey.GetPosition(), option.rect.height() - ( trackNormalizedValue * option.rect.height() ));

        painterPath.moveTo(firstKeyPos);

        for(int keyIndex = 0; keyIndex < inFloatTrack->GetKeyCount()-1; ++keyIndex)
        {
            TrackKey<float>& currentKey = inFloatTrack->GetKey(keyIndex);
            TrackKey<float>& nextKey = inFloatTrack->GetKey(keyIndex+1);

            float currentKeyNormalizedValue = NormalizeDataValue(0.0f, 100.0f, currentKey.GetData());
            float nextKeyNormalizedValue = NormalizeDataValue(0.0f, 100.0f, nextKey.GetData());

            QPoint currentKeyPos = QPoint(UIConstants::SecondSizeInPixels*currentKey.GetPosition(), option.rect.height() - ( currentKeyNormalizedValue * option.rect.height() ));
            QPoint nextKeyPos = QPoint(UIConstants::SecondSizeInPixels*nextKey.GetPosition(), option.rect.height() - ( nextKeyNormalizedValue * option.rect.height() ));
            painter->drawEllipse(currentKeyPos, 10, 10);
            painterPath.lineTo(nextKeyPos);
        }

        painter->drawPath(painterPath);
    }

    painter->restore();
}
