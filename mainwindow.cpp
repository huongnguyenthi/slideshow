#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "slideshowwidget.h"
#include <QWidget>
#include <QLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
  //  ui->setupUi(this);
    SlideShowWidget *slideshowWidget = new SlideShowWidget();
    setCentralWidget(slideshowWidget);
 }
MainWindow::~MainWindow()
{
    delete ui;
}

