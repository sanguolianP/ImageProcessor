#include "imageprocesser.h"

#include <iostream>
using namespace std;

imageprocesser::imageprocesser(QImage img)
{
    this->img=img;
}


QImage imageprocesser::rgb2Gray()
{
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            int gray=(r+g+b)/3;
            img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return img;
}

QImage imageprocesser::rgb2Bw(int threshold)
{
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            int gray=(r+g+b)/3;
            if(gray>=threshold)
              img.setPixel(i,j,qRgb(255,255,255));
            else
              img.setPixel(i,j,qRgb(0,0,0));
        }
    }
    return img;
}

QImage imageprocesser::colorReverse()
{
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int r=1-qRed(rgb);
            int g=1-qGreen(rgb);
            int b=1-qBlue(rgb);
            img.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return img;
}

QImage imageprocesser::contrastStretch(int m, double E)
{
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            int gray=(r+g+b)/3;
            if(gray==0)
                gray=1;
            gray=255/(1+qPow(m/gray,E));
            img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return img;
}

QImage imageprocesser::logTransform(double c)
{
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int r=qRed(rgb);
            int g=qGreen(rgb);
            int b=qBlue(rgb);
            int gray = r*0.299+ g*0.587 +b*0.114;
            gray = c*qLn(1+gray)/qLn(10);
            if(gray>255)
                gray=255;
            if(gray<0)
                gray=0;
            img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return img;
}

QImage imageprocesser::histeq()
{
    int width=img.width();
    int height=img.height();
    int N=width*height;
    int hist[256]={0};
    map<int,int> histMap;
    int sum=0;
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            hist[gray]++;
        }
    }
    for(int i=0;i<256;i++)
    {
        sum+=hist[i];
        histMap[i]=round(255*((double)sum/N));
    }
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            int result=histMap[gray];
            img.setPixel(i,j,qRgb(result,result,result));
        }
    }
    return img;
}

QImage imageprocesser::exactHisteq()
{
    int width=img.width();
    int height=img.height();
    vector<vector<sixdim> > pixels(256);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int gray=getGray(i,j);
            int a1=gray;
            int a2=a1+calcaA2(i,j);
            int a3=a2+calcaA3(i,j);
            int a4=a3+calcaA4(i,j);
            int a5=a4+calcaA5(i,j);
            int a6=a5+calcaA6(i,j);
            pixels[gray].push_back(sixdim(i,j,a1,a2/5.0,a3/9.0,a4/13.0,a5/21.0,a6/25.0));
        }
    }
    for(int i=0;i<256;i++)
    mySort<sixdim>(pixels[i].begin(),pixels[i].end());
    int N=width*height;
    int numPerGray=N/256;
    int current=0;
    for(int i=0;i<256;i++)
    {
        for(int j=0;j<pixels[i].size();j++)
        {
            int w=pixels[i][j].getX();
            int h=pixels[i][j].getY();
            int gray=current/numPerGray;
            if(gray>256) gray=255;
            img.setPixel(w,h,qRgb(gray,gray,gray));
            current++;
        }
    }
    return img;
}

int imageprocesser::getGray(int i, int j)
{
    QRgb rgb=img.pixel(i,j);
    int gray=(qRed(rgb)+qBlue(rgb)+qGreen(rgb))/3;
    return gray;
}

int imageprocesser::calcaA2(int i, int j)
{
    int sum=0;
    if(img.valid(i,j-1))
        sum+=getGray(i,j-1);
    if(img.valid(i,j+1))
        sum+=getGray(i,j+1);


    if(img.valid(i-1,j))
        sum+=getGray(i-1,j);
    if(img.valid(i+1,j))
        sum+=getGray(i+1,j);
    return sum;
}

int imageprocesser::calcaA3(int i, int j)
{
    int sum=0;
    if(img.valid(i-1,j-1))
        sum+=getGray(i-1,j-1);
    if(img.valid(i-1,j+1))
        sum+=getGray(i-1,j+1);


    if(img.valid(i+1,j+1))
        sum+=getGray(i+1,j+1);
    if(img.valid(i+1,j-1))
        sum+=getGray(i+1,j-1);
    return sum;
}

int imageprocesser::calcaA4(int i, int j)
{
    int sum=0;
    if(img.valid(i-2,j))
        sum+=getGray(i-2,j);
    if(img.valid(i+2,j))
        sum+=getGray(i+2,j);


    if(img.valid(i,j-2))
        sum+=getGray(i,j-2);
    if(img.valid(i,j+2))
        sum+=getGray(i,j+2);
    return sum;
}

int imageprocesser::calcaA5(int i, int j)
{
    int sum=0;
    if(img.valid(i-2,j-1))
        sum+=getGray(i-2,j-1);
    if(img.valid(i-2,j+1))
        sum+=getGray(i-2,j+1);

    if(img.valid(i+2,j-1))
        sum+=getGray(i+2,j-1);
    if(img.valid(i+2,j+1))
        sum+=getGray(i+2,j+1);


    if(img.valid(i-1,j-2))
        sum+=getGray(i-1,j-2);
    if(img.valid(i+1,j-2))
        sum+=getGray(i+1,j-2);

    if(img.valid(i-1,j+2))
        sum+=getGray(i-1,j+2);
    if(img.valid(i+1,j+2))
        sum+=getGray(i+1,j+2);
    return sum;
}

int imageprocesser::calcaA6(int i, int j)
{
    int sum=0;
    if(img.valid(i-2,j-2))
        sum+=getGray(i-2,j-2);
    if(img.valid(i-2,j+2))
        sum+=getGray(i-2,j+2);


    if(img.valid(i+2,j+2))
        sum+=getGray(i+2,j+2);
    if(img.valid(i+2,j-2))
        sum+=getGray(i+2,j-2);
    return sum;
}

template <typename T>
void imageprocesser::mySort(typename std::vector<T>::iterator begin,
                            typename std::vector<T>::iterator end) const
{
    if(begin==end) return;
    typename vector<T>::iterator iter1=begin;
    typename vector<T>::iterator iter2=end-1;
    while(iter1!=iter2)
    {
        int d=distance(iter1,iter2);
        if(d>0)
        {
            if(*iter1<*iter2)
            {
                iter2--;
                continue;
            }
            else
            {
                T tmp1=*iter1;
                *iter1=*iter2;
                *iter2=tmp1;
                typename vector<T>::iterator tmp2=iter1;
                iter1=iter2;
                iter2=tmp2;
                iter2++;
            }
        }
        else
        {
            if(*iter2<*iter1)
            {
                iter2++;
            }
            else
            {
                T tmp1=*iter1;
                *iter1=*iter2;
                *iter2=tmp1;
                typename vector<T>::iterator tmp2=iter1;
                iter1=iter2;
                iter2=tmp2;
                iter2--;
            }
        }
    }
    mySort<T>(begin,iter1);
    mySort<T>(iter1+1,end);
}

QImage imageprocesser::spatialFilter1(vector<double> data, int nCols)
{
    int nRows=data.size()/nCols;
    if(nRows%2!=1||nCols%2!=1)
    {
        qDebug()<<"nCols and nRows should be odd!"<<endl;
        return img;
    }
    zeroPadding(nCols,nRows);
    filter(data,nCols);
    crop(nCols,nRows);
    return img;
}

QImage imageprocesser::spatialFilter2(vector<double> data, int nCols)
{
    int nRows=data.size()/nCols;
    if(nRows%2!=1||nCols%2!=1)
    {
        qDebug()<<"nCols and nRows should be odd!"<<endl;
        return img;
    }
    duplicatePadding(nCols,nRows);
    filter(data,nCols);
    crop(nCols,nRows);
    return img;
}

QImage imageprocesser::spatialFilter3(vector<double> data, int nCols)
{
    int nRows=data.size()/nCols;
    if(nRows%2!=1||nCols%2!=1)
    {
        qDebug()<<"nCols and nRows should be odd!"<<endl;
        return img;
    }
    mirrorPadding(nCols,nRows);
    filter(data,nCols);
    crop(nCols,nRows);
    return img;
}

QImage imageprocesser::medianFilter1(int nCols, int nRows)
{
    if(nRows%2!=1||nCols%2!=1)
    {
        qDebug()<<"nCols and nRows should be odd!"<<endl;
        return img;
    }
    mirrorPadding(nCols,nRows);
    medianFilter(nCols,nRows);
    crop(nCols,nRows);
    return img;
}

QImage imageprocesser::meadianFilter(int nCols)
{
    QImage image(img);
    int nRows  = nCols;
    duplicatePadding(nCols,nRows);
    int width  = img.width();
    int height = img.height();
    for(int i=nCols/2;i<width-nCols/2;i++)
    {
        for(int j=nRows/2;j<height-nRows/2;j++)
        {
            vector<int> patch= getPatch(i,j,nRows,nCols);
            mySort<int>(patch.begin(),patch.end());
            int sum =patch[patch.size()/2-1];
            image.setPixel(i,j,qRgb(sum,sum,sum));
        }
    }
    img = image;
    crop(nCols,nRows);
    return img;
}

void imageprocesser::zeroPadding(int nCols, int nRows)
{
    QImage image=QImage(img.width()+nCols-1,img.height()+nRows-1,img.format());
    int width=image.width();
    int height=image.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(i<nCols/2||i>=nCols/2+img.width()||j<nRows/2||j>=nRows/2+img.height())
            {
                image.setPixel(i,j,qRgb(0,0,0));
            }
            else
            {
                QRgb rgb=img.pixel(i-nCols/2,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    img=image;
}

void imageprocesser::duplicatePadding(int nCols, int nRows)
{
    QImage image=QImage(img.width()+nCols-1,img.height()+nRows-1,img.format());
    int width=image.width();
    int height=image.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(i<nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(0,0);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(img.width()-1,0);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i<nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(0,img.height()-1);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(img.width()-1,img.height()-1);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }

            if(i<nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(0,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(img.width()-1,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=nCols/2&&i<width-nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,0);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=nCols/2&&i<width-nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,img.height()-1);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }

            if(i>=nCols/2&&i<width-nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    img=image;
}

void imageprocesser::mirrorPadding(int nCols, int nRows)
{
    QImage image=QImage(img.width()+nCols-1,img.height()+nRows-1,img.format());
    int width=image.width();
    int height=image.height();
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            if(i<nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(nCols/2-i,nRows/2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(2*width-(3*(nCols-1))/2-2-i,nRows/2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i<nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(nCols/2-i,2*height-(3*(nRows-1))/2-2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(2*width-(3*(nCols-1))/2-2-i,2*height-(3*(nRows-1))/2-2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }

            if(i<nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(nCols/2-i,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=width-nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(2*width-(3*(nCols-1))/2-2-i,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=nCols/2&&i<width-nCols/2&&j<nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,nRows/2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
            if(i>=nCols/2&&i<width-nCols/2&&j>=height-nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,2*height-(3*(nRows-1))/2-2-j);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }

            if(i>=nCols/2&&i<width-nCols/2&&j>=nRows/2&&j<height-nRows/2)
            {
                QRgb rgb=img.pixel(i-nCols/2,j-nRows/2);
                int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
                image.setPixel(i,j,qRgb(gray,gray,gray));
            }
        }
    }
    img=image;
}

void imageprocesser::filter(vector<double> data, int nCols)
{
    QImage image=QImage(img);
    int nRows=data.size()/nCols;
    int width=img.width();
    int height=img.height();
    for(int i=nCols/2;i<width-nCols/2;i++)
    {
        for(int j=nRows/2;j<height-nRows/2;j++)
        {
            vector<int> patch=getPatch(i,j,nCols,nRows);
            double sum=0;
            for(int k=0;k<data.size();k++)
            {
                sum+=patch[k]*data[k];
            }
            image.setPixel(i,j,qRgb(sum,sum,sum));
        }
    }
}

void imageprocesser::medianFilter(int nCols, int nRows)
{
    QImage image=QImage(img);
    int width=img.width();
    int height=img.height();
    for(int i=nCols/2;i<width-nCols/2;i++)
    {
        for(int j=nRows/2;j<height-nRows/2;j++)
        {
            vector<int> patch=getPatch(i,j,nCols,nRows);
            mySort<int>(patch.begin(),patch.end());
            double median=patch[patch.size()/2-1];
            image.setPixel(i,j,qRgb(median,median,median));
        }
    }
}

void imageprocesser::crop(int nCols, int nRows)
{
    QImage image(img.width()-nCols+1,img.height()-nRows+1,img.format());
    int width=img.width();
    int height=img.height();
    for(int i=nCols/2;i<width-nCols/2;i++)
    {
        for(int j=nRows/2;j<height-nRows/2;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            image.setPixel(i-nCols/2,j-nRows/2,qRgb(gray,gray,gray));
        }
    }
    img=image;
}

vector<int> imageprocesser::getPatch(int i, int j, int nCols, int nRows)
{
    vector<int> patch;
    for(int p=j-nRows/2;p<=j+nRows/2;p++)
    {
        for(int q=i-nCols/2;q<=i+nCols/2;q++)
        {
            QRgb rgb=img.pixel(q,p);
            int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            patch.push_back(gray);
        }
    }
    return patch;
}


QImage imageprocesser::fft2d()
{
    matrix data=fft2d(imgToMatrix(img));
    QImage fft2dImg=matrixToImg(data);
    img=fft2dImg;
    return img;
}

QImage imageprocesser::ifft2d()
{
    matrix _data=fft2d(imgToMatrix(img));
    matrix data=ifft2d(_data);
    QImage ifft2dImg=matrixToImg(data);
    img=ifft2dImg;
    return img;
}

matrix imageprocesser::imgToMatrix(QImage img)
{
    int width=img.width();
    int height=img.height();
    matrix temp(width, height, complex<double>(0,0));
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            QRgb rgb=img.pixel(i,j);
            int gray=(qRed(rgb)+qGreen(rgb)+qBlue(rgb))/3;
            temp(i,j)=complex<double>(gray,0);
        }
    }
    return temp;
}

QImage imageprocesser::matrixToImg(matrix mtx)
{
    int width=mtx.getRow(0).size();
    int height=mtx.getCol(0).size();
    QImage temp(width,height,img.format());
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            int tmp=qSqrt(qPow(mtx(i,j).real(),2)+qPow(mtx(i,j).imag(),2));
            temp.setPixel(i,j,qRgb(tmp,tmp,tmp));
        }
    }
    return temp;
}

size_t imageprocesser::calcN(size_t length)
{
    if (0==(length & (length-1))) {
        return length;
    }
    size_t temp = length;
    while (temp >>= 1) {
        length |= temp;
    }
    return length+1;
}

complex<double> imageprocesser::pow(complex<double> base, int exponent)
{
    complex<double> ret = 1;
    for (int i=0; i<exponent; ++i) {
        ret *= base;
    }
    return ret;
}

vector<complex<double> > imageprocesser::fft(vector<complex<double> > data, size_t N)
{
    if (0==N) {
        N = calcN(data.size());
    }
    if (0!=(N & (N-1))) {
        cout << "error N" << endl;
        return data;
    }

    // append 0 if necessary
    size_t delta = N - data.size();
    while(delta--) {
        data.push_back(complex<double>(0,0));
    }


    vector<complex<double> > ret;
    if (0 == N) {
        return ret;
    } else if(1 == N) {
        return data;
    } else if (2 == N) {
        // calculate if only two elements in data_
        ret.push_back(data[0]+data[1]);
        ret.push_back(data[0]-data[1]);
        return ret;
    } else {
        // digui
        vector<complex<double> > odd, even, oddRet, evenRet;
        for (size_t i = 0; i<data.size(); i += 2) {
            even.push_back(data[i]);
            odd.push_back(data[i+1]);
        }
        evenRet = fft(even);
        oddRet = fft(odd);

        // order the output
        complex<double> w_k1(cos(-2*PI/N), sin(-2*PI/N));
        for(size_t i=0; i<N/2; ++i) {
            ret.push_back(evenRet[i]+oddRet[i]*pow(w_k1, i));
        }
        for(size_t i=N/2; i<N; ++i) {
            size_t ii = i-N/2;
            ret.push_back(evenRet[ii]-oddRet[ii]*pow(w_k1, ii));
        }
        return ret;
    }
}

matrix imageprocesser::fft2d(const matrix &data, size_t M, size_t N)
{
    // dertermin M and N
    if (0 == M) M = calcN(data.Rows());
    if (0 == N) N = calcN(data.Cols());
    if ( (0 != (M & (M-1))) |
         (0 != (N & (N-1))) ) {
        cout << "error M or N" << endl;
        return data;
    }
    // append 0
    matrix temp(M, N, complex<double>(0,0));
    for (size_t i = 0; i<M; ++i) {
        for (size_t j = 0; j<N; ++j) {
            try {
                temp(i, j) = data(i, j);
            } catch (range_error e) {
                continue;
            }
        }
    }
    return fftRow(fftRow(temp).T()).T();
}

matrix imageprocesser::fftRow(const matrix &data)
{
    matrix ret(data);
    for (size_t i=0; i<ret.Rows(); ++i) {
        ret.setRow(i, fft(data.getRow(i)) );
    }
    return ret;
}

vector<complex<double> > imageprocesser::ifft(vector<complex<double> > data, size_t N)
{
    // determin N
    if(0==N)
    {
        N=calcN(data.size());
    }
    if(0!=(N&(N-1)))
    {
        cout<<"error N!"<<endl;
        return data;
    }

    // append 0 if necessary
    size_t delta=N-data.size();
    while(delta--)
    {
        data.push_back(complex<double>(0,0));
    }


    // do the calculation
    vector<complex<double> > res;
        // when N=0
    if(0==N)
        return data;
        // when N=1
    if(1==N)
        return data;
        // when N=2
    if(N==2)
    {
        res.push_back((data[0]+data[1]));
        res.push_back((data[0]-data[1]));
    }
        // when N>2
    if(N>2)
    {
        vector<complex<double> > odd,even,oddRet,evenRet;
        for(size_t i=0;i<N;i+=2)
        {
            even.push_back(data[i]);
            odd.push_back(data[i+1]);
        }
        evenRet=ifft(even);
        oddRet=ifft(odd);
        complex<double> w_N(cos(-2*PI/N),sin(-2*PI/N));
        complex<double> yet = complex<double>(1,0);
        for(size_t i=0;i<N/2;i++)
        {
            res.push_back((evenRet[i]+oddRet[i]*(yet/pow(w_N,i))));
        }
        for(size_t i=0;i<N/2;i++)
        {
            res.push_back((evenRet[i]-oddRet[i]*(yet/pow(w_N,i))));
        }
    }
    return res;
}

vector<complex<double> > imageprocesser::m_ifft(vector<complex<double> > data)
{
    vector<complex<double> > ret=ifft(data);
    int N = ret.size();
    complex<double> net = complex<double>(N,0);
    for(int i=0;i<N;i++)
    {
        ret[i]=ret[i]/net;
    }
    return ret;
}

matrix imageprocesser::ifft2d(const matrix &data, size_t M, size_t N)
{
    // dertermin M and N
    if (0 == M) M = calcN(data.Rows());
    if (0 == N) N = calcN(data.Cols());
    if ( (0 != (M & (M-1))) |
         (0 != (N & (N-1))) ) {
        cout << "error M or N" << endl;
        return data;
    }
    // append 0
    matrix temp(M, N, complex<double>(0,0));
    for (size_t i = 0; i<M; ++i) {
        for (size_t j = 0; j<N; ++j) {
            try {
                temp(i, j) = data(i, j);
            } catch (range_error e) {
                continue;
            }
        }
    }
    return ifftRow(ifftRow(temp).T()).T();
}

matrix imageprocesser::ifftRow(const matrix &data)
{
    matrix ret(data);
    for (size_t i=0; i<ret.Rows(); ++i) {
        ret.setRow(i, m_ifft(data.getRow(i)) );
    }
    return ret;
}

QImage& imageprocesser::transform(const QMatrix &tform)
{
    // check if tform is invertible
    bool invertible = true;
    QMatrix inv_tform = tform.inverted(&invertible);
    if (!invertible)    return img;

    // determine the size of new image
    double minX, maxX, minY, maxY;
    getRange(tform, minX, maxX, minY, maxY);

    // and create new QImage
    int newWidth = ceil(maxX-minX);
    int newHeight = ceil(maxY-minY);
    QImage newImg( newWidth, newHeight, img.format());

    // backward map per pixel
    for(int i=0;i<newWidth;++i)
    {
        for(int j=0;j<newHeight;++j)
        {
            QRgb newRgb = getNewRgb(inv_tform, i, j, minX, minY);
            newImg.setPixel(i,j, newRgb);
        }
    }
    img = newImg;
    return img;
}

void imageprocesser::getRange(const QMatrix &tform, double& minX,
                              double &maxX, double& minY, double& maxY) const
{
    // get four points in transformed image
    vector<qreal> vx(4),vy(4);
    tform.map(0,0,&vx[0],&vy[0]);
    tform.map(0,img.height()-1,&vx[1],&vy[1]);
    tform.map(img.width()-1,0,&vx[2],&vy[2]);
    tform.map(img.width()-1,img.height()-1,&vx[3],&vy[3]);

    // sort x and y
    mySort<qreal>(vx.begin(),vx.end());
    mySort<qreal>(vy.begin(),vy.end());

    // get the min and max of x and y
    minX = vx[0];
    minY = vy[0];
    maxX = vx[3];
    maxY = vy[3];

}

QRgb imageprocesser::getNewRgb(const QMatrix &tform, int i, int j
                               ,double minX, double minY) const
{
    // get the position in origal image
    double srcX,srcY;
    tform.map(i+minX,j+minY,&srcX,&srcY);
    // if it is out of the original image
    if(srcX<0 || srcX>img.width()-1 || srcY<0 || srcY>img.height()-1)
    {
        return qRgb(0,0,0);
    }

    // get RGB by bilinear interpolation
    else
    {
        return bilinear(srcX,srcY);
    }

}

QRgb imageprocesser::bilinear(double x, double y) const
{
    // get four pixels
    int x1 = ceil(x);
    int x2 = floor(x);
    int y1 = ceil(y);
    int y2 = floor(y);
    QRgb rgb1 = img.pixel(x1,y1);
    QRgb rgb2 = img.pixel(x1,y2);
    QRgb rgb3 = img.pixel(x2,y1);
    QRgb rgb4 = img.pixel(x2,y2);


    // bilinear on red
    double temp1,temp2;
    temp1 = linear_inter(qRed(rgb1),qRed(rgb2),y1,y2,y);
    temp2 = linear_inter(qRed(rgb3),qRed(rgb4),y1,y2,y);
    double r = linear_inter(temp1,temp2,x1,x2,x);

    //bilinear on green
    temp1 = linear_inter(qGreen(rgb1),qGreen(rgb2),y1,y2,y);
    temp2 = linear_inter(qGreen(rgb3),qGreen(rgb4),y1,y2,y);
    double g = linear_inter(temp1,temp2,x1,x2,x);

    //bilinear on blue
    temp1 = linear_inter(qBlue(rgb1),qBlue(rgb2),y1,y2,y);
    temp2 = linear_inter(qBlue(rgb3),qBlue(rgb4),y1,y2,y);
    double b= linear_inter(temp1,temp2,x1,x2,x);

    return qRgb(r,g,b);
}

double imageprocesser::linear_inter(double v1, double v2, double c1,
                                    double c2, double c3) const
{
    if(c1 == c2)
        return v1;
    // follow the formula of linear interpolation
    return (c2-c3)/(c2-c1)*v1+(c3-c1)/(c2-c1)*v2;
}
QImage imageprocesser::relief()
{
    img=rgb2Gray();
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width-1;i++)
    {
        for(int j=0;j<height-1;j++)
        {
            QRgb rgb=img.pixel(i,j);
            QRgb rgb1=img.pixel(i+1,j+1);
            QRgb tmp=rgb-rgb1+qRgb(128,128,128);
            int r=qRed(tmp);
            int g=qGreen(tmp);
            int b=qBlue(tmp);
            r=qBound(0,r,255);
            g=qBound(0,g,255);
            b=qBound(0,b,255);
            img.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return img;
}

QImage imageprocesser::gaussianBlur()
{
    int kernel[5][5]={{0,0,1,0,0},
                      {0,1,3,1,0},
                      {1,3,7,3,1},
                      {0,1,3,1,0},
                      {0,0,1,0,0}
                     };
    int kerenelSize=5;
    int sumKerenel=27;
    QColor color;
    int r,g,b;
    for(int i=kerenelSize/2;i<img.width()-(kerenelSize/2);i++)
    {
        for(int j=kerenelSize/2;j<img.height()-(kerenelSize/2);j++)
        {
            r=0;g=0;b=0;
            for(int p=-kerenelSize/2;p<=kerenelSize/2;p++)
            {
                for(int q=-kerenelSize/2;q<=kerenelSize/2;q++)
                {
                    color=QColor(img.pixel(i+p,j+q));
                    r+=color.red()*kernel[kerenelSize/2+p][kerenelSize/2+q];
                    g+=color.green()*kernel[kerenelSize/2+p][kerenelSize/2+q];
                    b+=color.blue()*kernel[kerenelSize/2+p][kerenelSize/2+q];
                }
            }
            r=qBound(0,r/sumKerenel,255);
            g=qBound(0,g/sumKerenel,255);
            b=qBound(0,b/sumKerenel,255);
            img.setPixel(i,j,qRgb(r,g,b));
        }
    }
    return img;
}

QImage imageprocesser::oilPaint(int radius, int intensity)
{
    int width=img.width();
    int height=img.height();
    QImage tmp(width,height,img.format());
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height;y++)
        {
            int intensity_counter[intensity]={0};
            int sum_r[intensity]={0};
            int sum_g[intensity]={0};
            int sum_b[intensity]={0};
            for(int i=x-radius;i<=x+radius;i++)
            {
                for(int j=y-radius;j<=y+radius;j++)
                {
                    if(i>=0&&i<width&&j>=0&&j<height)
                    {
                        QRgb rgb=img.pixel(i,j);
                        int r=qRed(rgb);
                        int g=qGreen(rgb);
                        int b=qBlue(rgb);
                        int m_intensity=(r+g+b)/3*intensity/255;
                        intensity_counter[m_intensity]++;
                        sum_r[m_intensity]+=r;
                        sum_g[m_intensity]+=g;
                        sum_b[m_intensity]+=b;
                    }
                }
            }

            int max=0;
            for(int i=0;i<intensity;i++)
            {
                if(intensity_counter[i]>max)
                    max=intensity_counter[i];
            }
            int index=0;
            for(int i=0;i<intensity;i++)
            {
                if(intensity_counter[i]==max)
                {
                    index=i;
                    break;
                }
            }
            int tmp_r=sum_r[index]/intensity_counter[index];
            int tmp_g=sum_g[index]/intensity_counter[index];
            int tmp_b=sum_b[index]/intensity_counter[index];
            tmp.setPixel(x,y,qRgb(tmp_r,tmp_g,tmp_b));

        }
    }
    img=tmp;
    return img;
}

QImage imageprocesser::edgeExtraction()
{
    rgb2Gray();
    int width=img.width();
    int height=img.height();
    for(int i=0;i<width-1;i++)
    {
        for(int j=0;j<height-1;j++)
        {
            int gray=qSqrt(qPow(getGray(i,j)-getGray(i+1,j+1),2)+
                        qPow(getGray(i+1,j)-getGray(i,j+1),2));
            if(gray<50)
                gray=0;
            else
                gray=255;
            img.setPixel(i,j,qRgb(gray,gray,gray));
        }
    }
    return img;
}

QImage imageprocesser::swell(int n)
{
    rgb2Bw(n);
    int width=img.width();
    int height=img.height();
    QImage temp(img);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            for(int p=-1;p<2;p++)
            {
                for(int q=-1;q<2;q++)
                {
                    if(i+p>=0&&i+p<width&&j+q>=0&&j+q<height)
                    {
                        if(getGray(i+p,j+q)==0)
                            temp.setPixel(i,j,qRgb(0,0,0));
                    }
                }
            }
        }
    }
    img=temp;
    return img;
}

QImage imageprocesser::erosion(int n)
{
    rgb2Bw(n);
    int width=img.width();
    int height=img.height();
    QImage temp(img);
    for(int i=0;i<width;i++)
    {
        for(int j=0;j<height;j++)
        {
            for(int p=-1;p<2;p++)
            {
                for(int q=-1;q<2;q++)
                {
                    if(i+p>=0&&i+p<width&&j+q>=0&&j+q<height)
                    {
                        if(getGray(i+p,j+q)==255)
                            temp.setPixel(i,j,qRgb(255,255,255));
                    }
                }
            }
        }
    }
    img=temp;
    return img;
}

QImage imageprocesser::openOperation(int n)
{
    img=erosion(n);
    img=swell(n);
    return img;
}

QImage imageprocesser::closeOperation(int n)
{
    img=swell(n);
    img=erosion(n);
    return img;
}


