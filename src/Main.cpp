#include <QApplication>
#include <ui/MainWindow.h>

// Metatypes
#include <ui/TrackHandle.h>

int main(int argc, char** argv)
{
    QApplication app(argc, argv);

    qRegisterMetaType<TrackHandle>();

    MainWindow window;
    window.show();

    return app.exec();
}

