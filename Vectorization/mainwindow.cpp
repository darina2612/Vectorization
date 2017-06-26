#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <iostream>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(handleLoadImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::handleLoadImage()
{
    QFileDialog* fileDialog = new QFileDialog(this);

    QString filename = QFileDialog::getOpenFileName(fileDialog,"Select Image", "/home", "JPEG (*.jpg);BMP (*.bmp)");


}
