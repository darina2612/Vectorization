#include "imagetranformations.h"

#include <vector>
#include <utility>
#include <algorithm>

ImageTranformations::ImageTranformations(std::string imageFileName):
    binarizedImage(NULL),
    contouredImage(NULL),
    skeletonizedImage(NULL)
{
    originalImage = new cv::Mat(cv::imread(imageFileName, CV_LOAD_IMAGE_COLOR));

    assert(originalImage);
}

ImageTranformations::~ImageTranformations()
{
    int height = binarizedImage->size().height;

    delete originalImage;
    delete binarizedImage;
    delete contouredImage;
    delete skeletonizedImage;

    for(int i = 0; i < height; ++i)
    {
        delete distanceTransform[i];
        delete distanceTransformHelper[i];
    }

    delete distanceTransform;
    delete distanceTransformHelper;
}

cv::Mat* ImageTranformations::getOriginalImage()
{
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
    if(!skeletonizedImage)
    {
        createSceletonizedImage();
    }

    return skeletonizedImage;
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

int Min(int a, int b)
{
    return ((a < b ? a : b));
}

int Min3(int a, int b, int c)
{
    return Min(a, Min(b, c));
}

int ImageTranformations::f1(int i, int j, int value) const
{
    if(value == 0)
    {
        return 0;
    }

    if(value == 1 && (i != 0 || j != 0))
    {
        return Min(distanceTransformHelper[i - 1][j] + 1, distanceTransformHelper[i][j - 1] + 1);
    }

    return binarizedImage->size().width + binarizedImage->size().height;
}

int ImageTranformations::f2(int i, int j, int value) const
{
    if(i == binarizedImage->size().height - 1)
    {
        if(j == binarizedImage->size().width - 1)
        {
            return value;
        }

        return Min(value, distanceTransform[i][j + 1] + 1);
    }

    if(j == binarizedImage->size().width - 1)
    {
        return Min(value, distanceTransform[i + 1][j] + 1);
    }

    return Min3(value, distanceTransform[i + 1][j] + 1, distanceTransform[i][j + 1] + 1);
}


void ImageTranformations::createSceletonizedImage()
{
    if(!binarizedImage)
    {
        createBinarizedImage();
    }

    int width = binarizedImage->size().width;
    int height = binarizedImage->size().height;

    distanceTransformHelper = new int*[height];
    distanceTransform = new int*[height];

    for(int i = 0; i < height; ++i)
    {
        distanceTransform[i] = new int[width];
        distanceTransformHelper[i] = new int[width];
    }

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            distanceTransformHelper[i][j] = f1(i, j, isBlack(binarizedImage->at<cv::Vec4b>(i, j)));
        }
    }

    for(int i = height - 1; i >= 0; --i)
    {
        for(int j = width - 1; j >= 0; --j)
        {
            distanceTransform[i][j] = f2(i, j, distanceTransformHelper[i][j]);
        }
    }

    std::vector< std::pair<int, int> > skeleton;

    for(int i = 0; i < height; ++i)
    {
        for(int j = 0; j < width; ++j)
        {
            if(distanceTransform[i][j] == 0)
            {
                continue;
            }
            int value = distanceTransform[i][j] + 1;

            if((i > 0 && distanceTransform[i - 1][j] != value) &&
               (j < (width - 1) && distanceTransform[i][j + 1] != value) &&
               (i < (height - 1) && distanceTransform[i + 1][j] != value) &&
               (j > 0 && distanceTransform[i][j - 1] != value))
            {
                skeleton.push_back(std::make_pair(i, j));
            }
        }
    }

    skeletonizedImage = new cv::Mat(binarizedImage->size(), binarizedImage->type());
    if(!contouredImage)
    {
        createContouredImage();
    }
    contouredImage->copyTo(*skeletonizedImage);

    for(int i = 0; i < skeleton.size(); ++i)
    {
        skeletonizedImage->at<cv::Vec4b>(skeleton[i].first, skeleton[i].second) = cv::Vec4b(0, 255, 0, 255);
    }
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
void ImageTranformations::createSceletonizedImage()
{
    if(!binarizedImage)
    {
        createBinarizedImage();
    }

    skeletonizedImage = new cv::Mat(binarizedImage->clone());

    int width = binarizedImage->size().width;
    int height = binarizedImage->size().height;

    bool changed = true;
    cv::Mat copy;

    while(changed)
    {
        changed = false;
        copy = skeletonizedImage->clone();
        for(int i = 0; i < height; ++i)
        {
            for(int j = 0; j < width; ++j)
            {
                if(isBlack(copy.at<cv::Vec4b>(i, j)))
                {
                    bool x[9] = {0};
                    if(j < (width - 1))
                    {
                        x[1] = isBlack(skeletonizedImage->at<cv::Vec4b>(i, j + 1));
                    }
                    if( i > 0 && j < (width - 1))
                    {
                        x[2] = isBlack(skeletonizedImage->at<cv::Vec4b>(i - 1, j + 1));
                    }
                    if(i > 0)
                    {
                        x[3] = isBlack(skeletonizedImage->at<cv::Vec4b>(i - 1));
                    }
                    if(i > 0 && j > 0)
                    {
                        x[4] = isBlack(skeletonizedImage->at<cv::Vec4b>(i - 1, j - 1));
                    }
                    if(j > 0)
                    {
                        x[5] = isBlack(skeletonizedImage->at<cv::Vec4b>(i, j - 1));
                    }
                    if(i < (height - 1) && j > 0)
                    {
                        x[6] = isBlack(skeletonizedImage->at<cv::Vec4b>(i + 1, j - 1));
                    }
                    if(i < (height - 1))
                    {
                        x[7] = isBlack(skeletonizedImage->at<cv::Vec4b>(i + 1, j));
                    }
                    if(i < (height - 1) && j < (width - 1))
                    {
                        x[8] = isBlack(skeletonizedImage->at<cv::Vec4b>(i + 1, j + 1));
                    }

                    int h = 0;

                    for(int k = 0; k <= 4; ++k)
                    {
                        h += ((x[2 * k - 1] == 0) && ((x[2 * k - 1] == 1) || (x[2 * k + 1] == 1)));
                    }

                    if(h != 1)
                    {
                        continue;
                    }

                    int n1 = 0, n2 = 0;

                    for(int k = 0; k <= 4; ++k)
                    {
                        n1 += (x[2 * k - 1] || x[2 * k]);
                        n2 += (x[2 * k] || x[2 * k + 1]);
                    }

                    int min_ = (n1 < n2) ? n1 : n2;

                    if( min_ < 2 || min_ > 3)
                    {
                        continue;
                    }

                    if(((i + j) % 2 == 0) && !((x[2] || x[3] || x[8]) && x[1]))
                    {
                        skeletonizedImage->at<cv::Vec4b>(i, j) = cv::Vec4b(255, 255, 255, 255);
                        changed = true;
                    }

                    else if(((i + j) % 2 == 0) && !((x[6] || x[7] || x[4]) && x[5]))
                    {
                        skeletonizedImage->at<cv::Vec4b>(i, j) = cv::Vec4b(255, 255, 255, 255);
                        changed = true;
                    }


                }
            }
        }
    }
}
*/
