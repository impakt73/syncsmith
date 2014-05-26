#pragma once

#include <QMainWindow>
#include <QtGui>
#include <TrackItemDelegate.h>
#include <TrackListModel.h>
#include <QtMultimedia>

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

    void on_action_Load_File_triggered();
    void audio_file_buffer_ready();
    void audio_file_loaded();
    void audio_file_error(QAudioDecoder::Error error);

private:
    Ui::MainWindow *ui;

    QStringList mNumbers;
    QStringListModel* mModel;
    TrackListModel mTrackModel;
    TrackItemDelegate mTrackItemDelegate;
    QAudioDecoder mDecoder;
    std::vector<unsigned short> mAudioBuffer;
    std::vector<unsigned short> mScaledAudioBuffer;
    unsigned short mMinSample;
    unsigned short mMaxSample;
};
