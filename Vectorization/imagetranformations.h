#ifndef IMAGETRANFORMATIONS_H
#define IMAGETRANFORMATIONS_H

#include <string>
#include <utility>
#include <opencv2/opencv.hpp>

enum Direction
{
    NONE,
    LEFT,
    RIGHT,
    TOP,
    BOTTOM
};

class ImageTranformations
{
public:
    ImageTranformations(std::string imageFilename);
    ~ImageTranformations();

    cv::Mat* getOriginalImage();
    cv::Mat* getBinarizedImage();
    cv::Mat* getContouredImage();
    cv::Mat* getSkeletonizedImage();

private:

    void createBinarizedImage();
    void createContouredImage();
    void createSceletonizedImage();

    bool isBlack(const cv::Vec4b& v) const;
    bool isContour(int i, int j) const;

    cv::Mat* originalImage;
    cv::Mat* binarizedImage;
    cv::Mat* contouredImage;
    cv::Mat* skeletonizedImage;
};

#endif // IMAGETRANFORMATIONS_H
