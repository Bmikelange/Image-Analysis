#include<header/mainwindow.h>
#include "header/fileselector.h"
#include <string>
#include "header/matrice.h"
#include <QDebug>
#include <QDir>
#include <header/connexe.h>

// définis la direction d'une composante connexe
int calculate_dir(cv::Point p1, cv::Point p2)
{
    if(p1.x-p2.x<0 && p1.y-p2.y==0)
        return 0;
    else if(p1.x-p2.x<0 && p1.y-p2.y>0)
        return 1;
    else if(p1.x-p2.x==0 && p1.y-p2.y>0)
        return 2;
    else if(p1.x-p2.x>0 && p1.y-p2.y>0)
        return 3;
    else if(p1.x-p2.x>0 && p1.y-p2.y==0)
        return 4;
    else if(p1.x-p2.x>0 && p1.y-p2.y<0)
        return 5;
    else if(p1.x-p2.x==0 && p1.y-p2.y<0)
        return 6;
    else if(p1.x-p2.x<0 && p1.y-p2.y<0)
        return 7;
}

//parcours les différents chemin entre les composantes connexes en fonction de la direction
void rempli_chemin(std::vector<std::pair<std::vector<cv::Point>,int>> & cheminADessiner,int dir, cv::Point p1, int i, int j, int k)
{
   switch(dir){
       case 0 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x-1,p1.y));
            cheminADessiner[j].first.push_back(cv::Point(p1.x-1,p1.y+1));
            cheminADessiner[k].first.push_back(cv::Point(p1.x-1,p1.y-1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 1 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x-1,p1.y+1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x-1,p1.y));
            cheminADessiner[k].first.push_back(cv::Point(p1.x,p1.y+1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 2 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x,p1.y+1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x-1,p1.y+1));
            cheminADessiner[k].first.push_back(cv::Point(p1.x+1,p1.y+1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 3 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x+1,p1.y+1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x,p1.y+1));
            cheminADessiner[k].first.push_back(cv::Point(p1.x+1,p1.y));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 4 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x+1,p1.y));
            cheminADessiner[j].first.push_back(cv::Point(p1.x+1,p1.y+1));
            cheminADessiner[k].first.push_back(cv::Point(p1.x+1,p1.y-1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 5 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x+1,p1.y-1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x+1,p1.y));
            cheminADessiner[k].first.push_back(cv::Point(p1.x,p1.y-1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 6 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x,p1.y-1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x+1,p1.y-1));
            cheminADessiner[k].first.push_back(cv::Point(p1.x-1,p1.y-1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
       case 7 :
            cheminADessiner[i].first.push_back(cv::Point(p1.x-1,p1.y-1));
            cheminADessiner[j].first.push_back(cv::Point(p1.x-1,p1.y));
            cheminADessiner[k].first.push_back(cv::Point(p1.x,p1.y-1));
            cheminADessiner[i].second++;
            cheminADessiner[j].second+=2;
            cheminADessiner[k].second+=2;
            break;
    }
}

void MainWindow::relieConnexe(int sens)
{
    int dstMax=5;
    int dir=0;
    #pragma omp parallel for
    for(unsigned int i=0;i<connexes.size();i++)
    {
        std::vector<std::pair<std::vector<cv::Point>,int>> cheminADessiner;
        std::vector<cv::Point> p;
        std::vector<int> connecte;
        cheminADessiner.push_back(std::pair<std::vector<cv::Point>,int>(p,0));
        cheminADessiner.push_back(std::pair<std::vector<cv::Point>,int>(p,0));
        cheminADessiner.push_back(std::pair<std::vector<cv::Point>,int>(p,0));
        unsigned int size=connexes[i]->v.size()-1;
        if(size>0)
        {
            cv::Point x,xm1;
            if(sens==0)
            {
                x=connexes[i]->v[size];
                xm1=connexes[i]->v[size-1];
            }
            else {
                x=connexes[i]->v[0];
                xm1=connexes[i]->v[1];
            }
            dir=calculate_dir(x, xm1);
            cheminADessiner[0].first.push_back(x);
            cheminADessiner[1].first.push_back(x);
            cheminADessiner[2].first.push_back(x);
            rempli_chemin(cheminADessiner,dir, x ,0, 1, 2);
            int Cheminsize=cheminADessiner.size();
            int controle=1;
            while(controle)
            {
                controle=0;
                for(int j=0;j<Cheminsize;j++)
                {
                    int verifValeur=1;
                    cv::Point p1=cheminADessiner[j].first[cheminADessiner[j].first.size()-1];
                    cv::Point p3=cheminADessiner[j].first[cheminADessiner[j].first.size()-2];
                    for(unsigned int k=0;k<connexes.size();k++)
                    {
                        for(unsigned int m=0;m<connexes[k]->v.size();m++)
                        {
                            if(m != i)
                            {
                                cv::Point p2=connexes[k]->v[m];
                                if(p1.x==p2.x && p1.y==p2.y)
                                {
                                    verifValeur=0;
                                    int contient=0;
                                    for(unsigned int n=0; n<connecte.size();n++)
                                    {
                                        if(connecte[n]==j)
                                        {
                                            contient =1;
                                            break;
                                        }

                                    }
                                    if(!contient)
                                        connecte.push_back(j);
                                    break;
                                }
                            }
                        }
                        if(verifValeur == 0)
                            break;
                    }
                    if(cheminADessiner[j].second<dstMax && verifValeur)
                    {
                        int newdir=calculate_dir(p1, p3);
                        cheminADessiner.push_back(cheminADessiner[j]);connecte[j];
                        cheminADessiner.push_back(cheminADessiner[j]);
                        rempli_chemin(cheminADessiner,newdir, p1 ,j, cheminADessiner.size()-1, cheminADessiner.size()-2);
                        controle=1;
                    }
                }
                Cheminsize=cheminADessiner.size();
            }
            if(connecte.size()>0)
            {
                int minIndice=connecte[0];
                int minVal=cheminADessiner[connecte[0]].second;
                for(int j=1;j<connecte.size();j++)
                {
                    if(minVal>=cheminADessiner[connecte[j]].second)
                    {
                        minIndice=connecte[j];
                        minVal=cheminADessiner[connecte[j]].second;
                    }
                }
                for(int j=1;j<cheminADessiner[minIndice].first.size();j++)
                {
                    cv::Point result=cheminADessiner[minIndice].first[j];
                    resultat.at<uchar>(result.x,result.y)=255;
                }
            }
        }
        else {
            resultat.at<uchar>(connexes[i]->v[0])=0;
        }
    }
}

void MainWindow::drawConnexe() {
    cv::Mat Image;
    Image.create((imEntree->imgIn).size(),(imEntree->imgIn).type());
    Image=cv::Scalar::all(0);
    cv::RNG rng;
    int r,g,b;
    for(unsigned int i=0;i<connexes.size();i++)
    {
        r = rng.uniform(0, 256);
        g = rng.uniform(0, 256);
        b = rng.uniform(0, 256);
        for(unsigned int j=0;j<connexes[i]->v.size();j++)
        {
            Image.at<cv::Vec3b>(connexes[i]->v[j].x,connexes[i]->v[j].y)=cv::Vec3b(r,g,b);
        }
    }
    std::string s =QDir::homePath().toUtf8().constData();
    cv::imwrite(s+"/sortie.jpg", Image);
    Imsortie->setimgIn(Image,1);
}


void MainWindow::affiche_image(cv::Mat m)
{
    cv::Mat color,imgIn,imgOut;
    cv::cvtColor(m, color, cv::COLOR_GRAY2BGR);
    std::string s =QDir::homePath().toUtf8().constData();
    cv::imwrite(s+"/sortie.jpg", color);
    if(color.cols >= color.rows)
    {
        int i= color.rows/500;
        if(i%2==0)
        {
            i=i+1;
        }
        cv::GaussianBlur(color, imgOut, cv::Size(i, i),0);
        cv::resize(imgOut,imgIn , cv::Size(500,(int)((500.f/(float)color.cols)*(float)color.rows)),cv::INTER_AREA );
    }
    else {
        int i= color.cols/500;
        if(i%2==0)
        {
            i=i+3;
        }
        cv::GaussianBlur(color, imgOut, cv::Size(i, i),0);
        cv::resize(imgOut,imgIn , cv::Size((int)((500.f/(float)color.rows)*(float)color.cols),500),cv::INTER_AREA );
    }
    Imsortie->setimgIn(imgIn,0);
}

void MainWindow::runClosure() {

    relieConnexe(0);
    relieConnexe(1);
    me.create(resultat.size(),resultat.type());
    me=cv::Scalar::all(0);
    ComposanteConnexe(resultat,me);
    affiche_image(resultat);
}

void MainWindow::ComputeMorph() {
    cv::Mat Image;
    int tab[3][3];
    for(int i=0;i<3;i++)
        for(int j=0;j<3;j++)
        {
            tab[i][j]=1;
        }

    resultat = morph(tab, 0);
    resultat = morph(tab, 1);
    cv::Mat val = morph(tab, 1);
    resultat=resultat-val;

    me.create(resultat.size(),resultat.type());
    me=cv::Scalar::all(0);
    ComposanteConnexe(resultat,me);
    affiche_image(resultat);
}

cv::Mat MainWindow::morph(int tab[3][3], int type)
{
    cv::Mat temp;
    temp.create(resultat.size(),resultat.type());
    temp=cv::Scalar::all(0);
    for (int i = 1; i < resultat.rows - 1; i++)
    {
        for (int j = 1; j < resultat.cols - 1; j++)
        {
            uchar val=0;
            std::vector<uchar> saveResult;
            for (int x=-1; x<2;x++)
            {
                 for (int y=-1;y<2;y++)
                 {
                     if ( tab[x+1][y+1] == 1 ) {
                       saveResult.push_back(resultat.at<uchar>(i+x, j+y));
                      }
                 }
            }
            if(type==0)
            {
                uchar maxval=saveResult[0];
                for(unsigned int xi=1;xi<saveResult.size();xi++)
                    if(saveResult[xi]>maxval)
                        maxval=saveResult[xi];
                val=maxval;
            }
            if(type==1)
            {
                uchar minval=saveResult[0];
                for(unsigned int xi=1;xi<saveResult.size();xi++)
                    if(saveResult[xi]<minval)
                        minval=saveResult[xi];
                val=minval;
            }
            temp.at<uchar>(i,j)=val;
        }
    }
    return temp;
}

float MainWindow::calculateConv(matrice m, int tab[3][3], float sumv, std::vector<float> &gradient, std::vector<float> &direction)
{

    float v = 0;
    float dir = 0;
    m.convolution(tab, v);
    dir=std::atan(v) * 180 / M_PI;
    if (mode == 1)
    {
        float x = 0;
        matrice mat = m.rotateMatrice(2);
        mat.convolution(tab, x);
        v = sqrt(v * v + x * x);
        if(x!=0)
            dir = std::atan(v / x) * 180 / M_PI;
        else {
            dir=std::atan(v) * 180 / M_PI;
        }
    }
    if (mode == 2)
    {
        float mt=0;
        v = v * v;
        for (int mi = 1; mi < 4; mi++)
        {
            float x = 0;
            matrice mat = m.rotateMatrice(mi);
            mat.convolution(tab, x);
            v += x * x;
            mt+=x*x;
        }
        v = sqrt(v);
        mt = sqrt(mt);
        if(mt !=0)
            dir=std::atan(v/mt) * 180 / M_PI;
        else
            dir=std::atan(v) * 180 / M_PI;
    }

    v=abs(v);
    gradient.push_back(v);
    direction.push_back(dir);
    sumv += v;
    return sumv;
}


float MainWindow::firstPass(matrice m, cv::Mat Image, float sumv, std::vector<float> &gradient, std::vector<float> &direction)
{
    for (int i = 1; i < Image.rows - 1; i++)
    {
        for (int j = 1; j < Image.cols - 1; j++)
        {
            int tab[3][3];
            for (int m = 0; m < 3; m++)
                for (int n = 0; n < 3; n++)
                {
                    tab[m][n] = Image.at<uchar>(i - 1 + m, j - 1 + n);
                }
            sumv = calculateConv(m, tab, sumv, gradient, direction);
        }
    }
    return sumv;
}


// vérifie si le point traité est un maximum local dans la direction de son gradient
int maxima_locaux(int tab[3])
{
    int value = 0;
    if(tab[0]<=tab[1] && tab[1] >= tab[2])
        value=1;
    return value;
}

// génère la table qui permet de vérifier les maximas locaux
void make_table(int tab2[3], const std::vector<float> &gradient, const std::vector<float> &direction,cv::Mat Image, int i, int j)
{
    if((direction[(i-1)*(Image.cols-2)+(j-1)]>=67.5 && direction[(i-1)*(Image.cols-2)+(j-1)]<=112.5) || (direction[(i-1)*(Image.cols-2)+(j-1)]<=-67.5 && direction[(i-1)*(Image.cols-2)+(j-1)]>=-112.5))
    {
        if(i-2>=0)
            tab2[0]=gradient[(i-2)*(Image.cols-2)+(j-1)];
        else
            tab2[0]=0;
        tab2[1]=gradient[(i-1)*(Image.cols-2)+(j-1)];
        if(i<=Image.rows-1)
            tab2[2]=gradient[(i)*(Image.cols-2)+(j-1)];
        else
            tab2[2]=0;

    }
    else if((direction[(i-1)*(Image.cols-2)+(j-1)]>=157.5 && direction[(i-1)*(Image.cols-2)+(j-1)]<=-157.5) || (direction[(i-1)*(Image.cols-2)+(j-1)]<=22.5 && direction[(i-1)*(Image.cols-2)+(j-1)]>=-22.5))
    {
        if(j-2>=0)
            tab2[0]=gradient[(i-1)*(Image.cols-2)+j-2];
        else
            tab2[0]=0;
        tab2[1]=gradient[(i-1)*(Image.cols-2)+(j-1)];
        if(j+1<=Image.cols-1)
            tab2[2]=gradient[(i-1)*(Image.cols-2)+j];
        else
            tab2[2]=0;
    }
    else if((direction[(i-1)*(Image.cols-2)+(j-1)]>=22.5 && direction[(i-1)*(Image.cols-2)+(j-1)]<=67.5) || (direction[(i-1)*(Image.cols-2)+(j-1)]<=-112.5 && direction[(i-1)*(Image.cols-2)+(j-1)]>=-157.5))
    {
        if(j<=Image.cols-1 && i-2 >=0)
            tab2[0]=gradient[(i-2)*(Image.cols-2)+j];
        else
            tab2[0]=0;
        tab2[1]=gradient[(i-1)*(Image.cols-2)+j-1];
        if(j-2>=0 && i<=Image.rows-1)
            tab2[2]=gradient[(i)*(Image.cols-2)+j-2];
        else
            tab2[2]=0;
    }
    else if((direction[(i-1)*(Image.cols-2)+(j-1)]>=112.5 && direction[(i-1)*(Image.cols-2)+(j-1)]<=157.5) || (direction[(i-1)*(Image.cols-2)+(j-1)]<=-22.5 && direction[(i-1)*(Image.cols-2)+(j-1)]>=-67.5))
    {
        if(j-2>=0 && i-2 >=0)
            tab2[0]=gradient[(i-2)*(Image.cols-2)+j-2];
        else
            tab2[0]=0;
        tab2[1]=gradient[(i-1)*(Image.cols-2)+(j-1)];
        if(j<=Image.cols-1 && i<=Image.rows-1)
            tab2[2]=gradient[(i)*(Image.cols-2)+j];
        else
            tab2[2]=0;
    }
}

// pour la recherche de composante connexe
// définis et stock l'ensemble des voisins du pixel traités qui sont des bords de l'image
void computeNeighborhood(int tab[3][3], std::vector<cv::Point> & v,int mi,int mj, cv::Mat & ImageControle )
{
    for(int i=0;i<3;i++)
    {
        for(int j=0;j<3;j++)
        {
            if(tab[i][j]==255 && (i-1!=0 || j-1 !=0) && ImageControle.at<uchar>(mi+i-1,mj+j-1)==0)
            {
                v.push_back(cv::Point(mi+i-1,mj+j-1));
                ImageControle.at<uchar>(mi+i-1,mj+j-1)=1;
            }
        }
    }
}

void MainWindow::ComposanteConnexe(cv::Mat Image,cv::Mat m)
{
    for(unsigned int i=0;i<connexes.size();i++)
    {
        connexes[i]->v.resize(0);
    }
    connexes.resize(0);
    for(int i=1;i<Image.rows-1;i++)
        for(int j=1;j<Image.cols-1;j++)
            if(m.at<uchar>(i,j)==0 && Image.at<uchar>(i,j)==255)
            {
                connexe * vect=new connexe();
                m.at<uchar>(i,j)=1;
                vect->v.push_back(cv::Point(i,j));
                std::vector<cv::Point> v;
                int tab[3][3];
                for(int mi=-1;mi<2;mi++)
                    for(int mj=-1;mj<2;mj++)
                        tab[mi+1][mj+1]=Image.at<uchar>(i+mi,j+mj);
                int me=i;
                int ne=j;
                computeNeighborhood(tab,v,me,ne,m);
                int size=v.size();
                while( size )
                {
                    me=v[0].x;
                    ne=v[0].y;
                    vect->v.push_back(cv::Point(me,ne));
                    v.erase(v.begin());
                    m.at<uchar>(me,ne)=1;
                    for(int mi=-1;mi<2;mi++)
                        for(int mj=-1;mj<2;mj++)
                            tab[mi+1][mj+1]=Image.at<uchar>(me+mi,ne+mj);
                    computeNeighborhood(tab,v,me,ne,m);
                    size=v.size();
                }
                connexes.push_back(vect);
            }
}

cv::Mat MainWindow::secondPass(matrice m, cv::Mat Image, float sumv, int lowThreshold, int mlowThreshold, const std::vector<float> &gradient, const std::vector<float> &direction)
{
    cv::Mat mat = cv::Mat::zeros(Image.size(), Image.type());
    for (int i = 1; i < Image.rows - 1; i++)
    {
        for (int j = 1; j < Image.cols - 1; j++)
        {
            float v = gradient[(i-1)*(Image.cols - 2)+(j-1)];
            v = abs(v);
            int tab2[3];
            make_table(tab2,gradient,direction,Image,i,j);
            if (filtering_mode == 0)
            {
                if (v > sumv)
                {
                    if(maxima_locaux(tab2))
                        mat.at<uchar>(i, j) = 255;
                    else
                        mat.at<uchar>(i,j)= 0;
                }
                else
                {
                    mat.at<uchar>(i, j) = 0;
                }
            }
            if (filtering_mode == 1)
            {
                float sums=0;
                int count=0;
                for (int mi = -sizepatch; mi < sizepatch+1; mi++)
                    for (int mj = -sizepatch; mj < sizepatch+1; mj++)
                        if(i-1+mi>=0 && i-1+mi<Image.rows-1 && j-1+mj>=0 && j-1+mj<Image.cols-1 )
                            if (mi != 0 || mj != 0)
                            {
                                sums += gradient[(i -1 + mi) * (Image.cols - 2) + j -1 + mj];
                                count++;
                            }
                sums = sums /count;
                if (v > sums)
                {
                    if(maxima_locaux(tab2))
                        mat.at<uchar>(i, j) = 255;
                    else
                        mat.at<uchar>(i,j)= 0;
                }
                else
                {
                    mat.at<uchar>(i, j) = 0;
                }
            }
            if (filtering_mode == 2 || filtering_mode == 3)
            {
                int controle = 0;
                if (v >= lowThreshold && v < mlowThreshold)
                    for (int mi = -1; mi < 2; mi++)
                        for (int mj = -1; mj < 2; mj++)
                            if (mi != 0 || mj != 0)
                                if (mat.at<uchar>(i + mi, j + mj) == 255)
                                    controle = 1;
                if (v < lowThreshold)
                {
                    mat.at<uchar>(i, j) = 0;
                }
                if (v >= mlowThreshold || controle)
                {
                    if(maxima_locaux(tab2))
                        mat.at<uchar>(i, j) = 255;
                    else
                        mat.at<uchar>(i,j)= 0;
                }
            }
        }
    }
    return mat;
}

cv::Point MainWindow::calculKmean(cv::Mat Image, std::vector<float> gradient)
{
    cv::Mat ImageRes;
    ImageRes.create(Image.size(),Image.type());
    for(int i=1;i<Image.rows-1;i++)
    {
        for(int j=1;j<Image.cols-1;j++)
        {
            ImageRes.at<uchar>(i,j)=gradient[(i-1)*(Image.cols - 2)+(j-1)];
        }
    }
    cv::Mat label;
    cv::TermCriteria criteria = cv::TermCriteria((cv::TermCriteria::MAX_ITER) + (cv::TermCriteria::EPS), 10, 1.0);
    cv::Mat center;
    cv::Mat frame2;
    ImageRes.convertTo(frame2, CV_32F, 1/255.0);
    frame2 = frame2.reshape(1,frame2.total());
    cv::kmeans(frame2,2,label,criteria,3,cv::KMEANS_PP_CENTERS  ,center);
    cv::Point p1;
    p1.x=center.at<float>(0)*255;
    p1.y=center.at<float>(1)*255;
    return p1;
}


cv::Mat MainWindow::ApplyFilter(matrice m, cv::Mat Image, int lowThreshold, int mlowThreshold)
{

    float sumv = 0;
    std::vector<float> gradient;
    gradient.clear();
    std::vector<float> direction;
    direction.clear();
    sumv = firstPass(m, Image, sumv, gradient, direction);
    sumv = sumv / ((Image.rows - 1) * (Image.cols - 1));
    cv::Mat mat;
    if (filtering_mode == 3)
    {
       cv::Point p = calculKmean(Image,gradient);
       if(p.x>p.y)
       {
           mat = secondPass(m, Image, sumv, p.y, p.x, gradient, direction);
       }
       else
       {
           mat = secondPass(m, Image, sumv, p.x, p.y, gradient, direction);
       }
    }
    else {
         mat = secondPass(m, Image, sumv, lowThreshold, mlowThreshold, gradient, direction);
    }
    me.create(Image.size(),Image.type());
    me=cv::Scalar::all(0);
    ComposanteConnexe(mat,me);
    affiche_image(mat);
    resultat=mat.clone();
    return mat;
}

void MainWindow::images()
{
    cv::Mat filterimage = ApplyFilter(m, detected_edges, lowThreshold, max_threshold);
}

void MainWindow::execute()
{
    cv::Mat input_image;
    int tab[3][3];
    tab[0][0] = iv1;
    tab[0][1] = iv2;
    tab[0][2] = iv3;
    tab[1][0] = iv4;
    tab[1][1] = iv5;
    tab[1][2] = iv6;
    tab[2][0] = iv7;
    tab[2][1] = iv8;
    tab[2][2] = iv9;
    input_image=imEntree->imgIn;
    m=matrice();
    m = m.addM(tab);
    cv::Mat gray_image;
    if(input_image.channels()==3)
        cv::cvtColor(input_image, gray_image, cv::COLOR_BGR2GRAY);
    else {
       return;
    }
    cv::bilateralFilter(gray_image,detected_edges,5,75,75);
    images();
}


void MainWindow::Computehough()
{
    Hough H;
    cv::Mat m;
    m=H.ComputeLine(resultat, 1, CV_PI/180, 150,0);
    affiche_image(m);
}

void MainWindow::Computehoughseg()
{
    Hough H;
    cv::Mat m;
    m=H.ComputeSegment(resultat, 1, CV_PI/180, 0,10,10);
    affiche_image(m);
}

void MainWindow::ComputehoughCircle()
{
    Hough H;
    cv::Mat m;
    m=H.ComputeCircle(resultat, 20,150, CV_PI/180, 100);
    affiche_image(m);
}
