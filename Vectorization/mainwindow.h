#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <opencv2/opencv.hpp>
#include <QMainWindow>
#include <imageview.h>
#include <imagetranformations.h>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void handleLoadImage();
    void handleShowBinarized();
    void handleShowContoured();
    void handleShowSkeletonized();

private:

    QImage Mat2QImage(cv::Mat const& src);

    Ui::MainWindow * ui;
    ImageView* currentImage;

    ImageTranformations* currentImageTransformations;
};

#endif // MAINWINDOW_H
