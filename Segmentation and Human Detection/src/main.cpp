#include <opencv2/highgui.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/core/types.hpp>
#include "opencv2/objdetect.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/video.hpp"
#include "opencv2/core.hpp"
#include <opencv2/objdetect.hpp>
#include <opencv2/photo.hpp>
#include <omp.h>

#include <iostream>
#include <vector>
#include <queue>
#include "segmentation.hpp"
#include <cmath>
#include <ctime>
#include <string>

#define GRID_SIZE 4
#define CRITERIA_1 15
#define CRITERIA_2 80
#define TEST 1

// définie la couleur de la segmentation
std::vector<cv::Vec3b> colors;

//Segmentation

// affecte toutes les transformations de bases à l'image en vue de son traitement
void preprocessing(const cv::Mat &input_image, cv::Mat &image, float min_area)
{
    //cv::cvtColor(input_image, image, cv::COLOR_BGR2GRAY);
    cv::bilateralFilter(input_image, image, 8, 50, 30);
    float resize_factor = min_area / image.size().area();
    if (resize_factor < 1)
        cv::resize(image, image, cv::Size(), resize_factor, resize_factor);

    //cv::fastNlMeansDenoisingColored(image, image);
    //cv::addWeighted(input_image, 0.5, image, 0.2, 0, image);
    // image = input_image.clone();
}


// place des points d'intérets aléatoirement sur l'image
void seed_placing(cv::Mat &image, std::vector<Region> &regions, int grid_size)
{
    unsigned int id = 0;
    for (unsigned int i = 0; i < image.size().width - grid_size; i += grid_size)
    {
        for (unsigned int j = 0; j < image.size().height - grid_size; j += grid_size)
        {
            cv::Point2i seed(i + rand() % grid_size,
                             j + rand() % grid_size);
            Region r(id++);
            r.AddPixel(seed, image.at<cv::Vec3b>(seed));
            regions.push_back(r);
        }
    }
}

//crée l'image ou chaque région est dessinée avec la moyenne des couleur de chacun de ses pixels
void set_image_avg_color(cv::Mat &image, std::vector<Region> &regions, bool show_borders)
{
    for (auto &region : regions)
    {
        region.CalcAvg();
        cv::Vec3b color = region.GetColor();
        for (auto p : region.GetPixels())
            image.at<cv::Vec3b>(p) = color;
        if (show_borders)
        {
            for (auto p : region.GetBorderPixels())
                image.at<cv::Vec3b>(p) = cv::Vec3b();
        }
    }
}

//crée l'image ou chaque région est dessinée avec des couleurs aléatoires
void set_image_color(cv::Mat &image, std::vector<Region> &regions, bool *ControleBool, bool show_borders)
{
    int count = 0;
    for (auto &region : regions)
    {
        if (!ControleBool[region._id])
        {
            int r = 30 + rand() % 205;
            int g = 30 + rand() % 205;
            int b = 30 + rand() % 205;
            cv::Vec3b color = cv::Vec3b(b, g, r);
            colors.push_back(color);
            for (auto p : region.GetPixels())
                image.at<cv::Vec3b>(p) = color;
        }
        if (show_borders)
        {
            for (auto p : region.GetBorderPixels())
                image.at<cv::Vec3b>(p) = cv::Vec3b();
        }
    }
}

// crée nos régions avec les points d'intérets marqués et les fais grossir
// s'arrête quand les régions ne grossissent plus
void region_growing(cv::Mat &image, std::vector<Region> &regions, int criteria1, int criteria2)
{

    unsigned int regions_nb = regions.size();
    unsigned int width = image.size().width;
    unsigned int height = image.size().height;

    int pixels[image.size().width][image.size().height];
    for (int i = 0; i < image.size().width; ++i)
    {
        for (int j = 0; j < image.size().height; ++j)
            pixels[i][j] = -1;
    }
    int marked_count = 0;
    for (auto &region : regions)
    {
        cv::Point2i p = region.GetPixels().front();
        pixels[p.x][p.y] = region._id;
        region.AddMarkedPixel(p);
        marked_count++;
    }
    int count = marked_count;
    while (marked_count != 0)
    {
        for (auto &region : regions)
        {
            if (region.MarkedPixelEmpty())
                continue;
            cv::Point2i p = region.GetMarkedPixel();
            marked_count--;
            cv::Vec3i p_color = (cv::Vec3i)image.at<cv::Vec3b>(p);
            for (int ii = -1; ii < 2; ++ii)
            {
                for (int jj = -1; jj < 2; ++jj)
                {
                    cv::Point2i pp(p.x + ii, p.y + jj);
                    if (!(pp.x < 0 || pp.x >= width || pp.y < 0 || pp.y >= height))
                    {
                        cv::Vec3b pp_color = image.at<cv::Vec3b>(pp);
                        float distance = cv::norm((cv::Vec3i)pp_color, (cv::Vec3i)p_color);
                        if (pixels[pp.x][pp.y] == -1)
                        {
                            if (distance < criteria1)
                            {
                                region.AddPixel(pp, pp_color);
                                region.AddMarkedPixel(pp);
                                marked_count++;
                                count++;
                                pixels[pp.x][pp.y] = region._id;
                            }
                            else
                            {
                                region.AddBorderPixel(pp);
                            }
                        }
                        else
                        {
                            if (pixels[pp.x][pp.y] != region._id)
                            {
                                if (distance < criteria2)
                                    region.AddNeighbour(pixels[pp.x][pp.y]);
                                else
                                    region.AddBorderPixel(pp);
                            }
                        }
                    }
                }
            }
        }
    }
    for (auto &region : regions)
    {
        region.CalcAvg();
    }
    std::cout << "Nombre de pixels traités  : " << count << std::endl;
    std::cout << "Nombre total de pixels    : " << image.size().area() << std::endl;
    std::cout << "Completion                : " << (float)count / image.size().area() * 100 << "%" << std::endl;
    std::cout << "Nombre de region avant fusion : " << regions.size() << std::endl;
}

//fusionne les régions entre elle quand elles sont de couleurs prochent
void region_merging(cv::Mat &image, std::vector<Region> &regions, int criteria2, bool show_borders)
{
    cv::Mat testimg(image.size(), image.type(), cv::Scalar(0, 0, 0));
    float distance;
    bool ControleBool[regions.size()] = {0};
    unsigned int count = 0;
    for (auto &region : regions)
    {
        bool change = true;
        while (change && !ControleBool[region._id])
        {
            change = false;
            std::set<unsigned int> neighbours = region.GetNeighbours();
            for (auto n = neighbours.begin(); n != neighbours.end(); n++)
            {
                Region neighbour = regions.at(*n);
                distance = cv::norm(region.GetColor(), neighbour.GetColor());
                if ((distance < criteria2 || neighbour.GetPixels().size() < 1) && !ControleBool[neighbour._id])
                {
                    change = true;
                    count++;
                    ControleBool[*n] = 1;

                    std::set<unsigned int> neighbours_of_absorbed = region.AbsorbRegion(neighbour);
                    for (auto id : neighbours_of_absorbed)
                    {
                        if (id != region._id)
                        {
                            Region noa = regions.at(id);
                            noa.ChangeNeighbour(*n, region._id);
                        }
                    }
                }
            }
        }
    }
    std::cout << "Nombre de region après fusion : " << regions.size() - count << std::endl;

    set_image_color(image, regions, ControleBool, show_borders);
}

// lance la boucle principale de segmentation et affiches les résultats
void segmentation(const cv::Mat &input_image, cv::Mat &output_image, bool show_borders, int grid_size,
                  int criteria1, int criteria2, float min_area, bool save)
{
    std::vector<Region> regions;
    preprocessing(input_image, output_image, min_area);
    seed_placing(output_image, regions, grid_size);
    region_growing(output_image, regions, criteria1, criteria2);
    cv::Mat output2_image = output_image.clone();
    region_merging(output_image, regions, criteria2, show_borders);
    set_image_avg_color(output2_image, regions, show_borders);

    if (output_image.size().area() < min_area)
    {
        cv::resize(output_image, output_image, cv::Size(), 2, 2, cv::InterpolationFlags::INTER_NEAREST);
        cv::resize(output2_image, output2_image, cv::Size(), 2, 2, cv::InterpolationFlags::INTER_NEAREST);
    }

    if (save) {
    cv::imwrite("./output/output.jpg", output_image);
    cv::imwrite("./output/output2.jpg", output2_image);
    } else
    {
        cv::imshow("frame1", output_image);
        cv::imshow("frame2", output2_image);
    }
    
}

//calcule un mask des zones en mouvement
cv::Mat farnerback(cv::Mat & gray1, cv::Mat & grayValue)
{
    cv::Mat flow(gray1.size(), CV_32FC2);
    cv::blur(gray1,gray1,cv::Size(10,10));
    cv::blur(grayValue,grayValue,cv::Size(10,10));
    cv::calcOpticalFlowFarneback(gray1, grayValue, flow, 0.5, 3, 50, 3, 5, 1.2, 0);
    // visualization
    cv::Mat flow_parts[2];
    cv::split(flow, flow_parts);
    cv::Mat magnitude, angle, magn_norm;
    cv::cartToPolar(flow_parts[0], flow_parts[1], magnitude, angle, true);
    cv::normalize(magnitude, magn_norm, 0.0f, 5.0f, cv::NORM_MINMAX);
    angle *= ((1.f / 360.f) * (180.f / 255.f));
    //build hsv image
    cv::Mat _hsv[3], hsv, hsv8, bgr;
    _hsv[0] = angle;
    _hsv[1] = cv::Mat::ones(angle.size(), CV_32F);
    _hsv[2] = magn_norm;
    cv::merge(_hsv, 3, hsv);
    hsv.convertTo(hsv8, CV_8U, 255.0);
    cv::cvtColor(hsv8, bgr, cv::COLOR_BGR2GRAY);
    cv::Mat bwimg;
    cv::threshold(bgr,bwimg, 175, 255, cv::THRESH_BINARY);
    gray1=grayValue.clone();
    return bwimg;
}


//lis la vidéo et exécute les diffférents traitements dessus
int capture(std::string access_file,unsigned int nbFrame)
{
    cv::VideoCapture capture(access_file);
    cv::Mat frameValue,frame1;
    cv::Mat grayValue,gray1;
    cv::Mat detected_edges;

    if(!capture.isOpened())
    {
        std::cout << "Could not open reference ";
        std::cout << access_file.data();
        std::cout << std::endl;
        return -1;
    }
    capture.read(frame1);
    unsigned int i=0;
    while(i < 60 + nbFrame){
        capture.read(frame1);
        i++;
    }
    cv::cvtColor(frame1, gray1, cv::COLOR_BGR2GRAY);
    i=0;
    int ws=0;
    while ( capture.read(frameValue) && i < 1000)
    {
        colors.resize(0);
        if( frameValue.empty() )
        {
            std::cout << "--(!) No captured frameValue -- Break!\n";
            break;
        }
        cv::Mat segmented_image;
        cv::Mat segmented_image_RT;
        cv::Mat m;
        cv::Mat save=frameValue.clone();
        cv::cvtColor(frameValue,grayValue,cv::COLOR_BGR2GRAY);
        #pragma omp parallel
        {
            #pragma omp single
	        {
                segmentation(frameValue, segmented_image, false, 2, 15, 24,250000, false);
                cv::resize(segmented_image, segmented_image_RT, frameValue.size(), 0, 0, cv::INTER_CUBIC);
            }
            #pragma omp single
	        {
                cv::fastNlMeansDenoisingColored(frameValue,frameValue,21,21, 21, 21);
                m = farnerback(gray1,grayValue);
            }
            #pragma omp barrier
        }
        cv::Mat binary;
        cv::Mat bin;
        cv::Vec3b p2;
        binary.create(gray1.size(),gray1.type());
        bin.create(gray1.size(),gray1.type());
        cv::Mat finalForeground;
        finalForeground.create(gray1.size(),gray1.type());
        finalForeground=cv::Scalar::all(0);
        for(int k=0;k<=colors.size();k++)
        {
            binary = cv::Scalar::all(0);
            float whitePixels=0;
            float whitePixels2=0;
            p2 = colors[k];
            cv::inRange(segmented_image_RT, p2, p2, binary);
            whitePixels = cv::countNonZero(binary);
            bin=binary & m;
            whitePixels2 = cv::countNonZero(bin);
            if( whitePixels>0 && whitePixels2/whitePixels>=0.5)
            {
                finalForeground = finalForeground+binary;
            }


        }
        cv::Mat finalResult;
        segmented_image_RT.copyTo(finalResult,finalForeground);
        cv::imshow("mask",finalForeground);
        cv::imshow("baseImage",save);
        cv::imshow("result",finalResult);
        if( cv::waitKey(10) == 27 )
        {
            break; // escape
        }
        i++;
    }
    capture.release();
    return 0;
}

//lance le mode vidéo
void video_Mode(std::string s)
{
    capture(s,200);

}

//fonction principale, lance la fonction de segmentation en fonction des paramètres et du type d'objet passé en argument
int main(int argc, char **argv)
{
    srand(time(NULL));
    float min_area = 250000;
    int criteria2 = 50;
    int criteria1 = 12;
    int grid_size = 10;
    std::string filename;
    cv::Mat input_image;
    bool show_borders = false;
    bool save = false;
    bool video = false;
    switch (argc)
    {
    case 8: 
        save = true;
    case 7:
        min_area = std::stof(argv[6]);
    case 6:
        criteria2 = std::stoi(argv[5]);
    case 5:
        criteria1 = std::stoi(argv[4]);
    case 4:
        grid_size = std::stoi(argv[3]);
    case 3:
        show_borders = std::stoi(argv[2]) != 0;
    case 2:
        filename = argv[1];
        if (filename.find(".jpg") != std::string::npos || filename.find(".png") != std::string::npos)
        {
            input_image = cv::imread(argv[1], cv::IMREAD_COLOR);

            if (!input_image.data)
            {
                std::cout << "Could not open or find the image" << std::endl;
                return -1;
            }
        }
        else if( filename.find(".mp4") != std::string::npos)
        {
            video=true;
        }
        else
        {
            std::cout << " Incorrect file format. \n Accepted format : \n- Image: .png, .jpg\n- Video: .mp4, .avi \n Exiting." << std::endl;
            return -1;
        }
        break;
    default:
        std::cout << " Usage: main FileToLoadAndDisplay [show_borders] [grid_size] [criteria1] [criteria2] [min_area] [save]" << std::endl;
        return -1;
    }
    if(!video)
    {
        cv::Mat segmented_image;
        segmentation(input_image, segmented_image, show_borders, grid_size, criteria1, criteria2, min_area, save);
        cv::waitKey(0);
    }
    else
    {
        video_Mode(filename);
    }
    

    return 0;
}
