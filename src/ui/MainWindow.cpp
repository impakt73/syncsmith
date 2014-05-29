#include <ui/MainWindow.h>
#include "ui_MainWindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mTrackItemDelegate(this),
    mDecoder(this),
    mSyncServer(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mNumbers << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "Seven" << "Eight" << "Nine" << "Ten" << "Eleven" << "Twelve";
    mModel = new QStringListModel(mNumbers);

    mTrackModel.addFloatTrack();

    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    QListView* listView2 = this->findChild<QListView*>("listView_2");
    listView->setModel(&mTrackModel);
    listView2->setModel(&mTrackModel);

    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, listView2->verticalScrollBar(), &QScrollBar::setValue);
    connect(listView->horizontalScrollBar(), &QScrollBar::valueChanged, timeline, &TimelineWidget::updateOffset);

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
    qDebug() << "Loading Complete! Audio Duration: " << mAudioBuffer.size() / 44100.0f;

    mScaledAudioBuffer.reserve(mAudioBuffer.size()/8820);
    unsigned short maxVal = mAudioBuffer[0];
    unsigned short minVal = mAudioBuffer[0];
    for(int sampleIndex = 0; sampleIndex < mAudioBuffer.size(); ++sampleIndex)
    {
        if(sampleIndex % 8820 == 0)
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
    timeline->updateOffset((int)inNewPosition);
    listView->horizontalScrollBar()->setValue((int)inNewPosition);
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
