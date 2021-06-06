#include "header/image.h"
#include <QWidget>
#include <QDebug>
#include <QObject>
#include <QtGui>
#include <QPixmap>
#include <QDir>
#include "header/fileselector.h"
#include <string>

// crée le widget d'affichage de l'image
image::image()
{
    this->setAutoFillBackground(true);
    this->setGeometry(0,0,500,500);
}

// initialise l'image et l'affiche
void image::setimgIn(cv::Mat im,int i)
{
    imgIn=im.clone();
    if(i==0)
        this->setFixedSize(imgIn.cols,imgIn.rows);
    else {
        if (im.cols >= im.rows)
            this->setFixedSize(500,(int)((500.f/(float)im.cols)*(float)im.rows));
        else
            this->setFixedSize((int)((500.f/(float)im.rows)*(float)im.cols),500);
    }
    QPalette palette;
    QImage img((uchar*)imgIn.data, imgIn.cols, imgIn.rows, static_cast<int>(imgIn.step), QImage::Format_RGB888);
    palette.setBrush(QPalette::Background, QPixmap::fromImage(img.rgbSwapped()).scaled(this->size()));
    this->setPalette(palette);
}


// met à jour l'affichage
void image::updateImage()
{
    QPalette palette;
    QImage img((uchar*)imgIn.data, imgIn.cols, imgIn.rows, static_cast<int>(imgIn.step), QImage::Format_RGB888);
    palette.setBrush(QPalette::Background, QPixmap::fromImage(img.rgbSwapped()).scaled(this->size()));
    this->setPalette(palette);
}

void image::mousePressEvent(QMouseEvent *event)
{
        cv::Mat m= cv::imread("/home/mikado/sortie.jpg");
        cv::imshow("affichage",m);
        cv::waitKey(0);
}
