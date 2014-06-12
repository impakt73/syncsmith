#include <ui/TrackHeaderItemDelegate.h>
#include <ui/TrackListModel.h>
#include <ui/TrackUtils.h>
#include <core/FloatTrack.h>
#include <QtGui>

TrackHeaderItemDelegate::TrackHeaderItemDelegate(QObject *parent)
    : QAbstractItemDelegate(parent)
{
}

TrackHeaderItemDelegate::~TrackHeaderItemDelegate()
{

}

void TrackHeaderItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    const Track* track = model->GetSyncContext()->GetTrack(index.row());

    TrackUtils::RenderTrackBackground(painter, option.rect, option.state & QStyle::State_Selected);

    if(track->GetType() == kTrackType_Float)
    {
        TrackUtils::RenderFloatTrackHeader(painter, option.rect, track);
    }
}

QSize TrackHeaderItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    return QSize(100, 100);
}

QWidget *TrackHeaderItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    // Edit trigger is also commented out
    /*
    const TrackListModel* model = static_cast<const TrackListModel*>(index.model());
    const Track* track = model->GetSyncContext()->GetTrack(index.row());

    if(track->GetType() == kTrackType_Float)
    {
        const FloatTrack* floatTrack = static_cast<const FloatTrack*>(track);

        QWidget* editorWidget = new QWidget(parent);
        editorWidget->setFixedSize(100, 125);
        editorWidget->setAutoFillBackground(true);
        editorWidget->setBackgroundRole(QPalette::Window);
        editorWidget->setFocusPolicy(Qt::StrongFocus);
        QBoxLayout* boxLayout = new QBoxLayout(QBoxLayout::Down, editorWidget);
        editorWidget->setLayout(boxLayout);
        QDoubleSpinBox* minValueSpinBox = new QDoubleSpinBox(editorWidget);
        QDoubleSpinBox* maxValueSpinBox = new QDoubleSpinBox(editorWidget);
        QFont textFont("Arial", 10);
        QLabel* titleLabel = new QLabel(editorWidget);
        QLabel* minLabel = new QLabel(editorWidget);
        QLabel* maxLabel = new QLabel(editorWidget);
        titleLabel->setText(QString(track->GetName().c_str()));
        titleLabel->setFont(textFont);
        minLabel->setText("Minimum");
        minLabel->setFont(textFont);
        maxLabel->setText("Maximum");
        maxLabel->setFont(textFont);
        minValueSpinBox->setRange(-1000.0, 1000.0);
        minValueSpinBox->setValue(floatTrack->GetMinValue());
        maxValueSpinBox->setRange(-1000.0, 1000.0);
        maxValueSpinBox->setValue(floatTrack->GetMaxValue());
        boxLayout->addWidget(titleLabel);
        boxLayout->addWidget(minLabel);
        boxLayout->addWidget(minValueSpinBox);
        boxLayout->addWidget(maxLabel);
        boxLayout->addWidget(maxValueSpinBox);

        return editorWidget;
    }
    */
}

void TrackHeaderItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{

}

void TrackHeaderItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{

}
