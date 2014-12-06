#include <QApplication>
#include <ui/MainWindow.h>

// Metatypes
#include <ui/TrackHandle.h>

extern "C"
{
#include <core/SyncTrack.h>
}

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

    for(unsigned int i = 0; i < 5; ++i)
    {
        SyncTrackKeyValue floatVal;
        floatVal.FloatData = static_cast<float>(i * 10.0f);
        SyncTrackUpdateKey(syncTrack, i * 2, kTrackInterpolationType_Linear, &floatVal);
        printf("Key At %dms: %.2f\n", i * 2, floatVal.FloatData);
    }
    for(unsigned int i = 0; i < 10; ++i)
    {
        float value = SyncTrackGetFloat(syncTrack, i);
        printf("Value At %dms: %.2f\n", i, value);
    }
    SyncTrackDestroyTrack(&syncTrack);

    return 0;
}

