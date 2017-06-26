#include "imagetranformations.h"

ImageTranformations::ImageTranformations(std::string imageFileName):
    binarizedImage(NULL)
{
    originalImage = new cv::Mat(cv::imread(imageFileName, CV_LOAD_IMAGE_COLOR));

    assert(originalImage);

    createBinarizedImage();
}

ImageTranformations::~ImageTranformations()
{
    delete originalImage;
    delete binarizedImage;
}

void ImageTranformations::createBinarizedImage()
{
    if(!binarizedImage)
    {
        cv::Mat graysaleImage = cv::Mat(originalImage->size(), CV_8U);

        //Convert BGR to Gray
        cv::cvtColor(*originalImage, graysaleImage, CV_BGR2GRAY);

        //Binary image
        binarizedImage =  new cv::Mat(graysaleImage.size(), graysaleImage.type());

        //Apply thresholding
        cv::threshold(graysaleImage, *binarizedImage, 100, 255, cv::THRESH_BINARY);

        cv::imshow("Image", *binarizedImage);
    }
}
