#ifndef IMAGETRANFORMATIONS_H
#define IMAGETRANFORMATIONS_H

#include <string>
#include <opencv2/opencv.hpp>

class ImageTranformations
{
public:
    ImageTranformations(std::string imageFilename);
    ~ImageTranformations();

private:

    void createBinarizedImage();

    cv::Mat* originalImage;
    cv::Mat* binarizedImage;
};

#endif // IMAGETRANFORMATIONS_H
