#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private:
    Ui::MainWindow * ui;
    ImageView* currentImage;

    ImageTranformations* currentImageTransformations;
};

#endif // MAINWINDOW_H
