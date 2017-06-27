#include "imagetranformations.h"

#include <vector>
#include <utility>

ImageTranformations::ImageTranformations(std::string imageFileName):
    binarizedImage(NULL),
    contouredImage(NULL)
{
    originalImage = new cv::Mat(cv::imread(imageFileName, CV_LOAD_IMAGE_COLOR));

    assert(originalImage);
}

ImageTranformations::~ImageTranformations()
{
    delete originalImage;
    delete binarizedImage;
    delete contouredImage;
}

cv::Mat* ImageTranformations::getOriginalImage()
{
    //cv::imshow("Image", *originalImage);

    return originalImage;
}

cv::Mat* ImageTranformations::getBinarizedImage()
{
    if(!binarizedImage)
    {
        createBinarizedImage();
    }

    return binarizedImage;
}

cv::Mat* ImageTranformations::getContouredImage()
{
    if(!contouredImage)
    {
        createContouredImage();
    }

    return contouredImage;
}

cv::Mat* ImageTranformations::getSkeletonizedImage()
{
    return NULL;
}

void ImageTranformations::createBinarizedImage()
{
    cv::Mat graysaleImage = cv::Mat(originalImage->size(), CV_8U);

    //Convert BGR to Gray
    cv::cvtColor(*originalImage, graysaleImage, CV_8U);

    //Binary image
    binarizedImage =  new cv::Mat(graysaleImage.size(), graysaleImage.type());

    //Apply thresholding
    cv::threshold(graysaleImage, *binarizedImage, 100, 255, cv::THRESH_BINARY);
}

void ImageTranformations::createContouredImage()
{
    if(!binarizedImage)
    {
        createBinarizedImage();
    }

    contouredImage = new cv::Mat(binarizedImage->size(), binarizedImage->type());
    contouredImage->setTo(255);

    std::vector< std::pair<int, int> > contour;

    int width = binarizedImage->size().width;
    int height = binarizedImage->size().height;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(isContour(i, j))
            {
                contour.push_back(std::make_pair(i, j));
            }
       }
    }

    for(int i = 0; i < contour.size(); ++i)
    {
        contouredImage->at<cv::Vec4b>(contour[i].first, contour[i].second) = cv::Vec4b(0, 0, 0, 255);
    }
}

void ImageTranformations::createSceletonizedImage()
{

}

bool ImageTranformations::isBlack(const cv::Vec4b& v) const
{
    return (v[0] == 0 && v[1] == 0 && v[2] == 0 && v[3] == 255);
}

bool ImageTranformations::isContour(int i, int j) const
{
   if(!isBlack(binarizedImage->at<cv::Vec4b>(i, j)))
   {
       return false;
   }

   int maxWidth = binarizedImage->size().width - 1, maxHeight = binarizedImage->size().height - 1;

   return ((i > 0 && j > 0 && !isBlack(binarizedImage->at<cv::Vec4b>(i - 1, j - 1))) ||
           (i > 0 && !isBlack(binarizedImage->at<cv::Vec4b>(i - 1, j))) ||
           (i > 0 && j < maxWidth && !isBlack(binarizedImage->at<cv::Vec4b>(i - 1, j + 1))) ||
           (j > 0 && !isBlack(binarizedImage->at<cv::Vec4b>(i, j - 1))) ||
           (j < maxWidth && !isBlack(binarizedImage->at<cv::Vec4b>(i, j + 1))) ||
           (i < maxHeight && j > 0 && !isBlack(binarizedImage->at<cv::Vec4b>(i + 1, j - 1))) ||
           (i < maxHeight && !isBlack(binarizedImage->at<cv::Vec4b>(i + 1, j))) ||
           (j < maxWidth && i < maxHeight && !isBlack(binarizedImage->at<cv::Vec4b>(i, j - 1))));
}

/*
    std::pair<int, int> helper, current = contour[0];

    while(true)
    {
        int currentRotations = 0;

        while(currentRotations <= 3)
        {
            helper = getLeft(current, currentDirecton);
            if(isBlack(binarizedImage->at<cv::Vec4b>(helper.first, helper.second)))
            {
                contour.push_back(getTop(current, currentDirecton));
                current = helper;
                contour.push_back(helper);
                rotateLeft(currentDirecton);
                break;
            }

            helper = getTop(current, currentDirecton);
            if(isBlack(binarizedImage->at<cv::Vec4b>(helper.first, helper.second)))
            {
                current = helper;
                contour.push_back(helper);
                break;
            }

            helper = getRight(current, currentDirecton);
            if(isBlack(binarizedImage->at<cv::Vec4b>(helper.first, helper.second)))
            {
                contour.push_back(getTop(current, currentDirecton));
                current = helper;
                contour.push_back(helper);

                //rotateRight(currentDirecton);
                break;
            }
            else
            {
                rotateRight(currentDirecton);
                ++currentRotations;
            }
        }
        if(currentRotations == 3 || (current.first == contour[0].first && current.second == contour[0].second))
        {
            break;
        }
    }
*/
/*
 * std::pair<int, int> ImageTranformations::getLeft(const std::pair<int, int>& pos, Direction d) const
{
    std::pair<int, int> left;

    switch (d)
    {
    case TOP:
        left = std::make_pair(pos.first - 1, pos.second - 1);
        break;
    case LEFT:
        left = std::make_pair(pos.first - 1, pos.second + 1);
        break;
    case BOTTOM:
        left = std::make_pair(pos.first + 1, pos.second + 1);
        break;
    case RIGHT:
        left = std::make_pair(pos.first + 1, pos.second - 1);
        break;
    }

    return left;
}

std::pair<int, int> ImageTranformations::getRight(const std::pair<int, int>& pos, Direction d) const
{
    std::pair<int, int> right;

    switch (d)
    {
    case TOP:
        right = std::make_pair(pos.first - 1, pos.second + 1);
        break;
    case LEFT:
        right = std::make_pair(pos.first + 1, pos.second + 1);
        break;
    case BOTTOM:
        right = std::make_pair(pos.first + 1, pos.second - 1);
        break;
    case RIGHT:
        right = std::make_pair(pos.first - 1, pos.second - 1);
        break;
    }

    return right;
}

std::pair<int, int> ImageTranformations::getTop(const std::pair<int, int>& pos, Direction d) const
{
    std::pair<int, int> top;

    switch (d)
    {
    case TOP:
        top = std::make_pair(pos.first - 1, pos.second);
        break;
    case LEFT:
        top = std::make_pair(pos.first, pos.second + 1);
        break;
    case BOTTOM:
        top = std::make_pair(pos.first + 1, pos.second);
        break;
    case RIGHT:
        top = std::make_pair(pos.first, pos.second - 1);
        break;
    }

    return top;
}

void ImageTranformations::rotateLeft(Direction& d) const
{
    switch (d)
    {
    case TOP:
        d = LEFT;
        break;
    case LEFT:
        d = BOTTOM;
        break;
    case BOTTOM:
        d = RIGHT;
        break;
    case RIGHT:
        d = TOP;
        break;
    default:
        break;
    }
}

void ImageTranformations::rotateRight(Direction& d) const
{
    switch (d)
    {
    case TOP:
        d = RIGHT;
        break;
    case LEFT:
        d = TOP;
        break;
    case BOTTOM:
        d = LEFT;
        break;
    case RIGHT:
        d = BOTTOM;
        break;
    default:
        break;
    }
}
*/
