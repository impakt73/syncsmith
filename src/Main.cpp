#include <QApplication>
#include <ui/MainWindow.h>

// Metatypes
#include <ui/TrackHandle.h>

extern "C"
{
#include <core/SyncTrack.h>
}

#include <core/TerminalSyncClient.h>
#include <stdio.h>

int main(int argc, char** argv)
{
    /*
    QApplication app(argc, argv);

    qRegisterMetaType<TrackHandle>();

    MainWindow window;
    window.show();

    return app.exec();
    */

    SyncTrack* syncTrack = SyncTrackCreateTrack("CatTrack", kTrackType_Float);

    for(unsigned int i = 1; i < 5; ++i)
    {
        SyncTrackKeyValue floatVal;
        floatVal.FloatData = static_cast<float>(i * 10.0f);
        SyncTrackUpdateKey(syncTrack, i * 5, kTrackInterpolationType_Linear, &floatVal);
        printf("Key At %dms: %.2f\n", i * 5, floatVal.FloatData);
    }

    SyncTrackKeyValue floatVal;
    floatVal.FloatData = static_cast<float>(0.0f);
    SyncTrackUpdateKey(syncTrack, 0, kTrackInterpolationType_Linear, &floatVal);

    SyncTrackRemoveKey(syncTrack, 0);

    for(unsigned int i = 0; i < 10; ++i)
    {
        float value = SyncTrackGetFloat(syncTrack, i);
        printf("Value At %dms: %.2f\n", i, value);
    }
    SyncTrackDestroyTrack(&syncTrack);

    TerminalSyncClient client;
    client.Initialize();
    client.Run();
    client.Shutdown();

    return 0;
}

