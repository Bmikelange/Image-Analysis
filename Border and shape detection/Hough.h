#ifndef HOOGH_H
#define HOOGH_H

#include <vector>
#include <opencv2/imgproc.hpp>

class Hough
{
public:
    Hough();
    std::vector<cv::Vec4i>  lines;
    cv::Mat ComputeLine(cv::Mat Image,float rho, float theta, float accept, int nbligneMax);
    cv::Mat ComputeSegment(cv::Mat Image,float rho, float theta, float accept,int lineLength, int lineGap);
    cv::Mat ComputeCircle(cv::Mat Image,float radius_min,float radius_max, float theta, float accept);
};

#endif // HOOGH_H
