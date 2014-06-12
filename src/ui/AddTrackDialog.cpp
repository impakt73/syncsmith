#include <ui/AddTrackDialog.h>
#include "ui_AddTrackDialog.h"

#include <core/Track.h>

QString TrackType2String(eTrackType inTrackType)
{
    switch(inTrackType)
    {
        case kTrackType_Float:
        {
            return "Float";
        }
        case kTrackType_Double:
        {
            return "Double";
        }
        case kTrackType_Integer:
        {
            return "Integer";
        }
        case kTrackType_Boolean:
        {
            return "Boolean";
        }
        case kTrackType_Count:
        {
            return "Count";
        }
    }

    return "Undefined";
}

AddTrackDialog::AddTrackDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddTrackDialog)
    , mTrackType(kTrackType_Float)
    , mTrackName("Undefined")
{
    ui->setupUi(this);

    QComboBox* trackTypeComboBox = this->findChild<QComboBox*>("cmbTrackType");
    for(unsigned int trackTypeIndex = 0; trackTypeIndex < kTrackType_Count; ++trackTypeIndex)
    {
        trackTypeComboBox->addItem(TrackType2String((eTrackType)trackTypeIndex));
    }
    trackTypeComboBox->setCurrentIndex(0);
}

AddTrackDialog::~AddTrackDialog()
{
    delete ui;
}

void AddTrackDialog::done(int result)
{
    if(result == QDialog::Accepted)
    {
        QComboBox* trackTypeComboBox = this->findChild<QComboBox*>("cmbTrackType");
        QLineEdit* trackNameLineEdit = this->findChild<QLineEdit*>("txtTrackName");

        mTrackType = (eTrackType)trackTypeComboBox->currentIndex();
        mTrackName = trackNameLineEdit->text();

        // Do Validation here

        QDialog::done(result);
    }
    else
    {
        QDialog::done(result);
    }
}
