#include "imageview.h"
#include <QImage>
#include <QLabel>
#include <QPixmap>
#include <QHBoxLayout>
#include <cassert>


ImageView::ImageView(const QImage im, QWidget* parent):
    QWidget(parent),
    image(new QImage(im)),
    imageLabel(NULL)
{
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

ImageView::~ImageView()
{
    delete image;
    delete imageLabel;
}
