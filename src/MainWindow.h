#pragma once

#include <QMainWindow>
#include <QtGui>
#include <TrackItemDelegate.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_actionE_xit_triggered();

private:
    Ui::MainWindow *ui;

    QStringList mNumbers;
    QStringListModel* mModel;
    TrackItemDelegate mTrackItemDelegate;
};
