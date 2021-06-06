#ifndef IMAGE_H
#define IMAGE_H

#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/core.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "QWidget"
#include "QMouseEvent"

// contient les fonctions nécessaires à l'affichage d'une image dans l'interface
class image : public QWidget
{
    Q_OBJECT
public:
    image();
    void updateImage();
    void mousePressEvent(QMouseEvent *event);
    void setimgIn(cv::Mat im,int i);
    cv::Mat imgIn;
};

#endif // IMAGE_H
