#include "Hough.h"
#include <functional>

Hough::Hough()
{

}

cv::Mat Hough::ComputeLine(cv::Mat Image,float rho, float theta,float accept, int nbligneMax)
{
    float invRho = 1 / rho;
    int numberAngle, numberRho;
    int width=Image.cols;
    int height=Image.rows;
    int n;
    const uchar* imageUchar;
    int pas;
    std::vector<double> tableauSin;
    std::vector<double> tableauCos;
    std::vector<int> Acc;

    imageUchar = Image.data;
    pas = Image.step;

    numberAngle = cvRound(CV_PI / theta);
    numberRho = cvRound(((width + height) * 2 + 1) / rho);

    Acc.clear();
    Acc.resize((numberAngle+2) * (numberRho+2),0);
    tableauSin.resize(numberAngle,0);
    tableauCos.resize(numberAngle,0);
    lines.resize(0);


    for( float ang = 0, n = 0; n < numberAngle; ang += theta, n++ )
    {
        tableauSin[n] = (double)(sin(ang) * invRho);
        tableauCos[n] = (double)(cos(ang) * invRho);
    }

    for( int i = 0; i < height; i++ )
        for( int j = 0; j < width; j++ )
            if( imageUchar[i * pas + j] != 0 )
                for( int n = 0; n < numberAngle; n++ )
                {
                    int r = cvRound( j * tableauCos[n] + i * tableauSin[n] );
                    r += (numberRho - 1) / 2;
                    Acc[(n+1) * (numberRho+2) + r+1]++;
                }


    std::vector<int> resultat;
    for( int r = 0; r < numberRho; r++ )
            for( int n = 0; n < numberAngle; n++ )
            {
                int pointBase = (n+1) * (numberRho+2) + r+1;
                if( Acc[pointBase] > accept &&
                    Acc[pointBase] > Acc[pointBase - 1] && Acc[pointBase] >= Acc[pointBase + 1] &&
                    Acc[pointBase] > Acc[pointBase - numberRho - 2] && Acc[pointBase] >= Acc[pointBase + numberRho + 2] )
                        resultat.push_back(pointBase);
            }

    if(nbligneMax>0)
    {
        std::sort(resultat.begin(),resultat.end(), std::greater<int>());
        if(nbligneMax<resultat.size())
            resultat.resize(nbligneMax);
    }

    double scale = 1./(numberRho+2);
    cv::Mat dst;
    dst.create(Image.size(),Image.type());
    dst=cv::Scalar::all(0);
    for( size_t i = 0; i < resultat.size(); i++ )
    {

        int idx = resultat[i];
        int n = cvFloor(idx*scale) - 1;
        int r = idx - (n+1)*(numberRho+2) - 1;
        double rhoResult = (r - (numberRho - 1)*0.5f) * rho, thetaResult = n * theta;
        cv::Point pt1, pt2;
        double a = cos(thetaResult), b = sin(thetaResult);
        double x0 = a*rhoResult, y0 = b*rhoResult;
        pt1.x = cvRound(x0 + 1000*(-b));
        pt1.y = cvRound(y0 + 1000*(a));
        pt2.x = cvRound(x0 - 1000*(-b));
        pt2.y = cvRound(y0 - 1000*(a));
        lines.push_back(cv::Vec4i(pt1.x,pt1.y,pt2.x,pt2.y));
        cv::line( dst, pt1, pt2, cv::Scalar(255,255,255), 1 , cv::LINE_AA);
    }

    return dst;
}


cv::Mat Hough::ComputeSegment(cv::Mat Image,float rho, float theta, float accept,int lineLength, int lineGap )
{
    cv::Mat mask;
    std::vector<float> tableauSin;
    std::vector<float> tableauCos;
    std::vector<cv::Vec4i> seq;
    int width, height;
    int numberAngle, numberRho;
    float ang;
    int r, n, count;
    float invRho = 1 / rho;
    cv::RNG rng = cv::RNG(-1);
    const float* tabSin;
    const float* tabCos;
    uchar* maskUchar;
    const uchar* imageUchar;
    int pas;
    cv::Mat Accum;

    width = Image.cols;
    height = Image.rows;

    numberAngle = cvRound((CV_PI+CV_PI/2) / theta);
    numberRho = cvRound(((width + height) * 2 + 1) / rho);

    Accum.create( numberAngle, numberRho, CV_32SC1 );
    mask.create( height, width, CV_8UC1 );
    tableauSin.resize(numberAngle,0);
    tableauCos.resize(numberAngle,0);
    Accum = cv::Scalar(0);
    lines.resize(0);

    for( ang = -CV_PI/2, n = 0; n < numberAngle; ang += theta, n++ )
    {
        tableauSin[n] = (float)(cos(ang) * invRho);
        tableauCos[n] = (float)(sin(ang) * invRho);
    }
    tabSin=&tableauSin[0];
    tabCos=&tableauCos[0];
    maskUchar=mask.data;
    imageUchar = Image.data;
    pas = Image.step;

    for( int i = 0; i < height; i++ )
    {
        uchar* mdata = maskUchar + i*width;
        for( int j = 0; j < width; j++ )
            if( imageUchar[i * pas + j] != 0 )
            {
                mdata[j] = (uchar)1;
                seq.push_back(cv::Vec4i(j,i,0,0));
            }
            else {
                mdata[j] = 0;
            }
    }

    for(  count = seq.size() ; count > 0; count-- )
    {
        int idx = rng() % count;
        int value_max = accept-1, n_max = 0;
        cv::Point pt = cv::Point(seq[idx][0],seq[idx][1]);
        cv::Point end_line[2] = {{0,0}, {0,0}};
        float a, b;
        int* AccumData = (int*)Accum.data;
        int i, j, k, x0, y0, dx0, dy0, xDrapeau;
        int bonne_ligne;
        const int sshift = 16;

        i = pt.y;
        j = pt.x;

        pt = cv::Point(seq[count-1][0],seq[count-1][1]);

        for( n = 0; n < numberAngle; n++, AccumData += numberRho )
        {
            r = cvRound( j * tabSin[n] + i * tabCos[n] );
            r += (numberRho - 1) / 2;
            int val = ++AccumData [r];
            if( value_max < val )
            {
                value_max = val;
                n_max = n;
            }
        }

        if( value_max < accept )
             continue;

        a = -tabCos[n_max];
        b = tabSin[n_max];
        x0 = j;
        y0 = i;
        if( fabs(a) > fabs(b) )
        {
            xDrapeau = 1;
            dx0 = a > 0 ? 1 : -1;
            dy0 = cvRound( b*(1 << sshift)/fabs(a) );
            y0 = (y0 << sshift) + (1 << (sshift-1));
        }
        else
        {
            xDrapeau = 0;
            dy0 = b > 0 ? 1 : -1;
            dx0 = cvRound( a*(1 << sshift)/fabs(b) );
            x0 = (x0 << sshift) + (1 << (sshift-1));
        }

        for( k = 0; k < 2; k++ )
        {
            int gap = 0, x = x0, y = y0, dx = dx0, dy = dy0;

            if( k > 0 )
                dx = -dx, dy = -dy;
            for( ;; x += dx, y += dy )
            {
                uchar* mdata;
                int i1, j1;

                if( xDrapeau )
                {
                    j1 = x;
                    i1 = y >> sshift;
                }
                else
                {
                    j1 = x >> sshift;
                    i1 = y;
                }

                if( j1 < 0 || j1 >= width || i1 < 0 || i1 >= height )
                    break;

                if( maskUchar[i1*width + j1] )
                {
                    gap = 0;
                    end_line[k].y = i1;
                    end_line[k].x = j1;
                }
                else if( ++gap > lineGap )
                    break;
            }
        }

        bonne_ligne = abs(end_line[1].x - end_line[0].x) >= lineLength ||
                    abs(end_line[1].y - end_line[0].y) >= lineLength;

        if( bonne_ligne )
        {
            cv::Vec4i result = { end_line[0].x, end_line[0].y, end_line[1].x, end_line[1].y };
            lines.push_back(result);
        }
    }

    cv::Mat dst;
    dst.create(Image.size(),Image.type());
    dst=cv::Scalar::all(0);
    for( size_t i = 0; i < lines.size(); i++ )
    {
        cv::line( dst, cv::Point(lines[i][0],lines[i][1]), cv::Point(lines[i][2],lines[i][3]), cv::Scalar(255,255,255), 1 , cv::LINE_AA);
    }
    return dst;
}


cv::Mat Hough::ComputeCircle(cv::Mat Image,float radius_min,float radius_max, float theta, float accept)
{
    int width=Image.cols;
    int height=Image.rows;
    int n;
    const uchar* imageUchar;
    int pas;
    int radius_diff=radius_max-radius_min;
    std::vector<std::vector<std::vector<int>>> Acc(height, std::vector<std::vector<int>>(width, std::vector<int>(radius_diff, 0)));

    imageUchar = Image.data;
    pas = Image.step;
    lines.resize(0);

    for(int r=0;r < radius_diff; r++)
        for( int i = 0; i < height; i++ )
            for( int j = 0; j < width; j++ )
                if( imageUchar[i * pas + j] != 0 )
                    for( float n = 0; n < 2.0f * M_PI; n+=theta )
                    {
                        int b = cvRound(j - (r+radius_min) * sin(n));
                        int a = cvRound(i - (r+radius_min) * cos(n));
                        if(a>=0 && b >=0 && a<height && b<width)
                            Acc[a][b][r]++;
                    }

    std::vector<cv::Vec3i> resultat;
    for(int r=1;r < radius_diff-1; r++)
        for( int a = 1; a < height-1; a++ )
            for( int b = 1; b < width-1; b++ )
            {
                if( Acc[a][b][r]> accept &&
                    Acc[a][b][r] > Acc[a][b][r-1] && Acc[a][b][r] > Acc[a][b][r+1] &&
                    Acc[a][b][r] > Acc[a][b-1][r] && Acc[a][b][r] > Acc[a][b+1][r] &&
                    Acc[a][b][r] > Acc[a-1][b][r] && Acc[a][b][r] > Acc[a+1][b][r] )
                        resultat.push_back(cv::Vec3i(b,a,r));
            }

    cv::Mat dst;
    dst.create(Image.size(),Image.type());
    dst=cv::Scalar::all(0);
    for( size_t i = 0; i < resultat.size(); i++ )
    {
            cv::Vec3i c = resultat[i];
            cv::Point center = cv::Point(c[0], c[1]);
            int radius = c[2]+radius_min;
            cv::circle( dst, center, radius, cv::Scalar(255,255,255), 1, cv::LINE_AA);
    }

    return dst;
}
