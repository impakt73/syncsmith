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

    TimelineWidget* timeline = this->findChild<TimelineWidget*>("widget");
    QListView* listView = this->findChild<QListView*>("listView");
    QListView* listView2 = this->findChild<QListView*>("listView_2");
    listView->setModel(mModel);
    listView2->setModel(mModel);
    listView2->setItemDelegate(listView->itemDelegate());

    connect(listView->verticalScrollBar(), &QScrollBar::valueChanged, listView2->verticalScrollBar(), &QScrollBar::setValue);
    connect(listView->horizontalScrollBar(), &QScrollBar::valueChanged, timeline, &TimelineWidget::updateOffset);
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
