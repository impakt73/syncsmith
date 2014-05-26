#include <TrackItemDelegate.h>
#include <QtGui>
#include <TrackListModel.h>

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
            painter->drawLine(option.rect.x() + sampleIndex*3, (option.rect.height()/2) - barLengthInPixels, option.rect.x() + sampleIndex*3, (option.rect.height()/2) + barLengthInPixels);
        }
    }

    painter->drawText(QPoint(option.rect.x() + option.rect.width() / 2, option.rect.y() + option.rect.height() / 2), qvariant_cast<QString>(index.data()));
    painter->restore();
}

QSize TrackItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(10000, 100);
}
