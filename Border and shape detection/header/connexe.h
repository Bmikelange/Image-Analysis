#ifndef CONNEXE_H
#define CONNEXE_H
#include <vector>

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/core.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/photo.hpp>

// contient une composante connexe
class connexe
{
public:
    std::vector<cv::Point> v;
    connexe();
};

#endif // CONNEXE_H
