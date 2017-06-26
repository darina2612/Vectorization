#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentImage(NULL)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(handleLoadImage()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentImage;
    delete currentImageTransformations;
}

void MainWindow::handleLoadImage()
{
    QFileDialog* fileDialog = new QFileDialog(this);

    QString filename = QFileDialog::getOpenFileName(fileDialog,"Select Image", "", "JPEG (*.jpg);BMP (*.bmp)");

    if(filename.size() == 0)
    {
        return;
    }

    currentImageTransformations = new ImageTranformations(filename.toStdString());

    currentImage = new ImageView(filename);

    currentImage->show();
}
