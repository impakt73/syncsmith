#ifndef ADDTRACKDIALOG_H
#define ADDTRACKDIALOG_H

#include <QDialog>
#include <core/Track.h>

namespace Ui {
class AddTrackDialog;
}

class AddTrackDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddTrackDialog(QWidget *parent = 0);
    ~AddTrackDialog();

    inline QString trackName(void) const { return mTrackName; }
    inline eTrackType trackType(void) const { return mTrackType; }

private:
    Ui::AddTrackDialog *ui;
    QString mTrackName;
    eTrackType mTrackType;

private slots:
    void done(int result);
};

#endif // ADDTRACKDIALOG_H
