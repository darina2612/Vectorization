#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QFileDialog>

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    currentImageTransformations(NULL)
{
    ui->setupUi(this);

    QObject::connect(ui->pushButton, SIGNAL(pressed()), this, SLOT(handleLoadImage()));
    QObject::connect(ui->pushButton_2, SIGNAL(pressed()), this, SLOT(handleShowBinarized()));
    QObject::connect(ui->pushButton_3, SIGNAL(pressed()), this, SLOT(handleShowContoured()));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete currentImageTransformations;
}

void MainWindow::handleLoadImage()
{
    QFileDialog* fileDialog = new QFileDialog(this);

    QString filename = QFileDialog::getOpenFileName(fileDialog,"Select Image", "");//, "JPEG (*.jpg, *.jpeg);BMP (*.bmp)");

    if(filename.size() == 0)
    {
        return;
    }

    currentImageTransformations = new ImageTranformations(filename.toStdString());

    QImage toShow = Mat2QImage(*currentImageTransformations->getOriginalImage());

    ImageView* view = new ImageView(toShow);

    view->show();
}

 void MainWindow::handleShowBinarized()
 {
     QImage toShow = Mat2QImage(*currentImageTransformations->getBinarizedImage());

     ImageView* view = new ImageView(toShow);

     view->show();
 }

 void MainWindow::handleShowContoured()
 {
     QImage toShow = Mat2QImage(*currentImageTransformations->getContouredImage());

     ImageView* view = new ImageView(toShow);

     view->show();
 }

 QImage MainWindow::Mat2QImage(cv::Mat const& src)
 {
      cv::Mat temp; // make the same cv::Mat
      cvtColor(src, temp,CV_BGR2RGB); // cvtColor Makes a copt, that what i need
      QImage dest((const uchar *) temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888);
      dest.bits(); // enforce deep copy, see documentation
      // of QImage::QImage ( const uchar * data, int width, int height, Format format )
      return dest;
 }
