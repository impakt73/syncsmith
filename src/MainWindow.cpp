#include <MainWindow.h>
#include "ui_MainWindow.h"

#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    mTrackItemDelegate(this),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    mNumbers << "One" << "Two" << "Three" << "Four" << "Five" << "Six" << "Seven" << "Eight" << "Nine" << "Ten" << "Eleven" << "Twelve";
    mModel = new QStringListModel(mNumbers);

    QListView* listView = this->findChild<QListView*>("listView");
    listView->setModel(mModel);
}

MainWindow::~MainWindow()
{
    delete mModel;

    delete ui;
}

void MainWindow::on_actionE_xit_triggered()
{
    this->close();
}
