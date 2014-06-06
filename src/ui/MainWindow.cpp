#include <ui/MainWindow.h>
#include "ui_MainWindow.h"
#include <ui/UIConstants.h>

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mTrackItemDelegate(this),
    mDecoder(this),
    mSyncServer(this),
    mTrackModel(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mNumbers << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "Seven" << "Eight" << "Nine" << "Ten" << "Eleven" << "Twelve";
    mModel = new QStringListModel(mNumbers);

    mTrackModel.SetSyncContext(mSyncServer.GetSyncContext());

    mTrackModel.addFloatTrack();

    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    QListView* listView2 = this->findChild<QListView*>("listView_2");
    listView->setModel(&mTrackModel);
    listView2->setModel(&mTrackModel);

    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, listView2->verticalScrollBar(), &QScrollBar::setValue);
    //connect(listView->horizontalScrollBar(), &QScrollBar::valueChanged, timeline, &TimelineWidget::updateOffset);

    connect(&mSyncServer, &SyncServer::PositionChanged, this, &MainWindow::on_position_changed);

    mSyncServer.Start();

    grabKeyboard();
}

MainWindow::~MainWindow()
{
    releaseKeyboard();

    mSyncServer.Stop();

    delete mModel;

    delete ui;
}

void MainWindow::on_actionE_xit_triggered()
{
    this->close();
}

void MainWindow::on_action_Load_File_triggered()
{
    QAudioFormat desiredFormat;
    desiredFormat.setChannelCount(1);
    desiredFormat.setCodec("audio/x-raw");
    desiredFormat.setSampleType(QAudioFormat::UnSignedInt);
    desiredFormat.setSampleRate(44100);
    desiredFormat.setSampleSize(16);

    mDecoder.setAudioFormat(desiredFormat);
    mDecoder.setSourceFilename("/home/greg/dev/cpp/syncsmith/bin/test2.mp3");

    connect(&mDecoder, &QAudioDecoder::bufferReady, this, &MainWindow::audio_file_buffer_ready);
    connect(&mDecoder, &QAudioDecoder::finished, this, &MainWindow::audio_file_loaded);
    connect(&mDecoder, SIGNAL(error(QAudioDecoder::Error)), this, SLOT(audio_file_error(QAudioDecoder::Error)));
    mDecoder.start();

    // Now wait for bufferReady() signal and call decoder->read()
}

void MainWindow::audio_file_buffer_ready()
{
    QAudioBuffer buffer = mDecoder.read();
    const unsigned short* bufferData = buffer.constData<unsigned short>();

    if(mDecoder.duration() != -1 && mAudioBuffer.empty())
    {
        mAudioBuffer.reserve((mDecoder.duration() / 1000.0f)*44100);
        mMinSample = bufferData[0];
        mMaxSample = bufferData[0];
    }

    for(int sampleIndex = 0; sampleIndex < buffer.sampleCount(); ++sampleIndex)
    {
        mAudioBuffer.push_back(bufferData[sampleIndex]);

        if(bufferData[sampleIndex] > mMaxSample)
            mMaxSample = bufferData[sampleIndex];

        if(bufferData[sampleIndex] < mMinSample)
            mMinSample = bufferData[sampleIndex];
    }

    qDebug() << "Read " << buffer.sampleCount() << " Samples from buffer at " << (buffer.startTime() / 1000.0f) / 1000.0f << "!";
}

void MainWindow::audio_file_loaded()
{
    double audioLengthInSeconds = mAudioBuffer.size() / 44100.0;
    qDebug() << "Loading Complete! Audio Duration: " << audioLengthInSeconds;

    mScaledAudioBuffer.reserve(mAudioBuffer.size()/17640);
    unsigned short maxVal = mAudioBuffer[0];
    unsigned short minVal = mAudioBuffer[0];
    for(int sampleIndex = 0; sampleIndex < mAudioBuffer.size(); ++sampleIndex)
    {
        if(sampleIndex % 17640 == 0)
        {
            mScaledAudioBuffer.push_back(maxVal);
            maxVal = mAudioBuffer[sampleIndex];
            minVal = mAudioBuffer[sampleIndex];
        }
        else
        {
            maxVal = std::max(mAudioBuffer[sampleIndex], maxVal);
            minVal = std::min(mAudioBuffer[sampleIndex], minVal);
        }
    }
    mAudioBuffer.clear();

    mTrackModel.setAudioSamples(&mScaledAudioBuffer);
    mTrackModel.setMinSample(mMinSample);
    mTrackModel.setMaxSample(mMaxSample);
    mTrackModel.setAudioLength(audioLengthInSeconds);
    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    timeline->setLength(audioLengthInSeconds);
}

void MainWindow::audio_file_error(QAudioDecoder::Error error)
{
    qDebug() << "Loading Failed! Error: " << mDecoder.errorString();
}

void MainWindow::on_action_Send_Message_triggered()
{
    mSyncServer.BroadcastMessage("Hello World!");
}

void MainWindow::on_position_changed(double inNewPosition)
{
    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    qDebug() << "Current Time In Seconds: " << inNewPosition;
    int pixelPos = inNewPosition * UIConstants::SecondSizeInPixels;
    timeline->updateScrollOffset(inNewPosition);
    //timeline->scroll(-pixelPos, 0);
    listView->horizontalScrollBar()->setValue(pixelPos);
}

void MainWindow::keyPressEvent(QKeyEvent* keycode)
{
    if(keycode->key() == Qt::Key_Space)
    {
        keycode->accept();
        mSyncServer.OnToggleAction();
    }
    else
    {
        keycode->ignore();
    }
}
