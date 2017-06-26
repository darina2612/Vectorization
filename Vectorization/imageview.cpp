#include "imageview.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <cassert>


ImageView::ImageView(const QString& filename, QWidget* parent):
    QWidget(parent),
    image(NULL),
    imageLabel(NULL)
{
    assert(loadImage(filename));
    createLabelWithPixmap(QPixmap::fromImage(*image));

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->addWidget(imageLabel);

    setLayout(layout);
    setFixedSize(image->width(), image->height());
}

void ImageView::createLabelWithPixmap(const QPixmap& pixmap)
{
    if (!imageLabel)
        imageLabel = new QLabel;

    imageLabel->setPixmap(pixmap);
}

bool ImageView::loadImage(const QString& imageName)
{
    if (!image)
        image = new QImage();

   return image->load(imageName);
}

ImageView::~ImageView()
{
    delete image;
    delete imageLabel;
}
