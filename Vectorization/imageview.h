#ifndef IMAGEVIEW_H
#define IMAGEVIEW_H

#include <QWidget>
#include <QImage>
#include <QLabel>

class QPixmap;

class ImageView : public QWidget
{
    Q_OBJECT

public:
    explicit ImageView(const QString& filename, QWidget *parent = 0);

    ~ImageView();

    QImage* getImage() const;

private:
    bool loadImage(const QString& imageName);
    void createLabelWithPixmap(const QPixmap& pixmap);

    QImage* image;
    QLabel* imageLabel;
};

#endif // IMAGEVIEW_H
