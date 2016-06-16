#ifndef IMAGEPROCESSER_H
#define IMAGEPROCESSER_H

#include <QImage>
#include <QRgb>
#include <QtCore/qmath.h>
#include <QList>
#include <QPoint>
#include <vector>
#include <iterator>
#include <complex>
#include <QDebug>
#include <QColor>
#include <QtMath>
#include "sixdim.h"
#include "matrix.h"

using namespace std;
#define PI 3.1415926

class imageprocesser
{
public:
    imageprocesser(QImage img);
    QImage img;
    QImage rgb2Gray();
    QImage rgb2Bw(int threshold);
    QImage colorReverse();
    QImage contrastStretch(int m,double E);
    QImage logTransform(double c);
    QImage histeq();
    QImage exactHisteq();

    QImage spatialFilter1(vector<double> data,int nCols);
    QImage spatialFilter2(vector<double> data,int nCols);
    QImage spatialFilter3(vector<double> data,int nCols);

    QImage medianFilter1(int nCols,int nRows);
    QImage meadianFilter(int nCols);
    QImage fft2d();
    QImage ifft2d();

    QImage& transform(const QMatrix& tform);

    QImage relief();
    QImage gaussianBlur();
    QImage oilPaint(int radius,int intensity);
    QImage edgeExtraction();
    QImage swell(int n);
    QImage erosion(int n);
    QImage openOperation(int n);
    QImage closeOperation(int n);


private:
    int getGray(int i,int j);
    int calcaA2(int i,int j);
    int calcaA3(int i,int j);
    int calcaA4(int i,int j);
    int calcaA5(int i,int j);
    int calcaA6(int i,int j);

    template<typename T>
    void mySort(typename std::vector<T>::iterator begin,
                typename std::vector<T>::iterator end) const;

    void zeroPadding(int nCols,int nRows);
    void duplicatePadding(int nCols,int nRows);
    void mirrorPadding(int nCols,int nRows);

    void filter(vector<double> data,int nCols);
    void medianFilter(int nCols,int nRows);
    vector<int> getPatch(int i,int j,int nCols,int nRows);
    void crop(int nCols,int nRows);

    matrix imgToMatrix(QImage img);
    QImage matrixToImg(matrix mtx);
    size_t calcN(size_t length);
    complex<double> pow(complex<double> base, int exponent);

    vector<complex<double> > fft(vector<complex<double> > data, size_t N=0);
    matrix fft2d(const matrix& data, size_t M=0, size_t N=0);
    matrix fftRow(const matrix& data);

    vector<complex<double> >  ifft(vector<complex<double> > data, size_t N=0);
    vector<complex<double> >  m_ifft(vector<complex<double> > data);
    matrix ifft2d(const matrix& data, size_t M=0, size_t N=0);
    matrix ifftRow(const matrix& data);


    void getRange(const QMatrix& tform, double &minX, double& maxX,
                  double& minY, double& maxY) const;
    QRgb bilinear(double x, double y) const;
    double linear_inter(double v1, double v2, double c1, double c2, double c3) const;
    QRgb getNewRgb(const QMatrix& tform, int i, int j, double minX, double minY) const;
};

#endif // IMAGEPROCESSER_H
