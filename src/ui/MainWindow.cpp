#include <ui/MainWindow.h>
#include "ui_MainWindow.h"
#include <ui/UIConstants.h>
#include <ui/AddTrackDialog.h>

#include <QScrollBar>
#include <QMessageBox>
#include <core/FloatTrack.h>

MainWindow* MainWindow::sInstance = nullptr;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mTrackItemDelegate(this),
    mDecoder(this),
    mTrackModel(this),
    mIsPlaying(false),
    mTimer(this),
    mStartTimeOffset(0.0),
    ui(new Ui::MainWindow)
{
    sInstance = this;

    ui->setupUi(this);

    mTimer.setInterval(16);
    mTimer.setTimerType(Qt::PreciseTimer);
    mStartTime = QDateTime::currentMSecsSinceEpoch();
    connect(&mTimer, &QTimer::timeout, this, &MainWindow::on_timer_tick);

    mNumbers << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "Seven" << "Eight" << "Nine" << "Ten" << "Eleven" << "Twelve";
    mModel = new QStringListModel(mNumbers);

    mTrackModel.SetSyncContext(&mSyncContext);

    setFocusPolicy(Qt::StrongFocus);

    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    QListView* listView2 = this->findChild<QListView*>("listView_2");
    listView->setModel(&mTrackModel);
    listView2->setModel(&mTrackModel);

    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, listView2->verticalScrollBar(), &QScrollBar::setValue);
    connect(listView->horizontalScrollBar(), &QScrollBar::sliderMoved, this, &MainWindow::on_hscroll);

    //connect(&mSyncServer, &SyncServer::PositionChanged, this, &MainWindow::on_position_changed);

    //mSyncServer.Start();
    connect(&mSyncClient, &SyncClient::Connected, this, &MainWindow::on_client_connected);
    connect(&mSyncClient, &SyncClient::PacketReceived, this, &MainWindow::on_packet_received);
    mSyncClient.Connect("127.0.0.1", 8000);
}

MainWindow::~MainWindow()
{
    //mSyncServer.Stop();
    mSyncClient.Disconnect();

    delete mModel;

    delete ui;
}

void MainWindow::OnKeyAdded(FloatTrack *inFloatTrack, double inPosition, float inData)
{
    SyncPacket_AddKey* packet = new SyncPacket_AddKey();
    packet->Type = kSyncPacketType_AddKey;
    packet->TrackName = QString::fromStdString(inFloatTrack->GetName());
    packet->Position = inPosition;
    packet->Data = inData;

    mSyncClient.SendPacket(packet);
}

void MainWindow::OnKeyModified(FloatTrack *inFloatTrack, double inPosition, float inData)
{
    SyncPacket_ModifyKey* packet = new SyncPacket_ModifyKey();
    packet->Type = kSyncPacketType_ModifyKey;
    packet->TrackName = QString::fromStdString(inFloatTrack->GetName());
    packet->Position = inPosition;
    packet->Data = inData;

    mSyncClient.SendPacket(packet);
}

void MainWindow::OnKeyRemoved(FloatTrack *inFloatTrack, double inPosition)
{
    SyncPacket_RemoveKey* packet = new SyncPacket_RemoveKey();
    packet->Type = kSyncPacketType_RemoveKey;
    packet->TrackName = QString::fromStdString(inFloatTrack->GetName());
    packet->Position = inPosition;

    mSyncClient.SendPacket(packet);
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
    //mSyncServer.BroadcastMessage("Hello World!");
}

void MainWindow::on_position_changed(double inNewPosition)
{
    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    qDebug() << "Current Time In Seconds: " << inNewPosition;
    int pixelPos = (float)inNewPosition * (float)UIConstants::SecondSizeInPixels;
    timeline->setPlaybackPosition(inNewPosition);
    listView->horizontalScrollBar()->setValue(timeline->scrollOffset() * UIConstants::SecondSizeInPixels);
    mSyncContext.SetPosition(inNewPosition);
}

void MainWindow::on_hscroll(int inScrollPos)
{
    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    float scrollPos = (float)inScrollPos / (float)UIConstants::SecondSizeInPixels;
    timeline->updateScrollOffset(scrollPos);
}

void MainWindow::keyPressEvent(QKeyEvent* keycode)
{
    if(keycode->key() == Qt::Key_Space)
    {
        keycode->accept();

        TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
        //mSyncServer.OnToggleAction(timeline->userPlaybackPosition());

        if(!mIsPlaying)
        {
            SyncPacket_Seek* seekPacket = new SyncPacket_Seek();
            seekPacket->Type = kSyncPacketType_Seek;
            seekPacket->Position = timeline->userPlaybackPosition();
            mSyncClient.SendPacket(seekPacket);

            SyncPacket_Play* packet = new SyncPacket_Play();
            packet->Type = kSyncPacketType_Play;
            mSyncClient.SendPacket(packet);

            mStartTimeOffset = timeline->userPlaybackPosition();
            mStartTime = QDateTime::currentMSecsSinceEpoch();

            mTimer.start();

            mIsPlaying = true;
        }
        else
        {
            SyncPacket_Pause* packet = new SyncPacket_Pause();
            packet->Type = kSyncPacketType_Pause;
            mSyncClient.SendPacket(packet);

            mTimer.stop();

            mIsPlaying = false;
        }
    }
    else
    {
        keycode->ignore();
    }
}

void MainWindow::on_action_Add_Track_triggered()
{
    AddTrackDialog* addTrackDialog = new AddTrackDialog(this);
    if(addTrackDialog->exec() == QDialog::Accepted)
    {
        std::string trackName = addTrackDialog->trackName().toStdString();
        eTrackType trackType = addTrackDialog->trackType();
        mTrackModel.addTrack(trackName, trackType);

        SyncPacket_AddTrack* packet = new SyncPacket_AddTrack();
        packet->Type = kSyncPacketType_AddTrack;
        packet->TrackName = QString::fromStdString(trackName);
        packet->TrackType = trackType;

        mSyncClient.SendPacket(packet);
    }
}

void MainWindow::on_client_connected()
{
    QMessageBox msgBox;
    msgBox.setText("Connected!");
    msgBox.exec();
}

void MainWindow::on_packet_received(SyncPacket* inPacket)
{
}

void MainWindow::on_timer_tick()
{
    double currentTimeInSeconds = (static_cast<double>(QDateTime::currentMSecsSinceEpoch() - mStartTime) / 1000.0) + mStartTimeOffset;
    on_position_changed(currentTimeInSeconds);
}

void MainWindow::on_action_Remove_Selected_Track_triggered()
{
    QListView* headerView = this->findChild<QListView*>("listView_2");
    unsigned int trackIndex = headerView->selectionModel()->currentIndex().row();
    TrackListModel* modelHandle = static_cast<TrackListModel*>(headerView->model());

    SyncPacket_RemoveTrack* packet = new SyncPacket_RemoveTrack();
    packet->Type = kSyncPacketType_RemoveTrack;
    packet->TrackName = QString::fromStdString(mSyncContext.GetTrack(trackIndex)->GetName());

    modelHandle->removeTrack(trackIndex);

    mSyncClient.SendPacket(packet);
}
