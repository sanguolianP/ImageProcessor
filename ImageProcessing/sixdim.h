#ifndef SIXDIM_H
#define SIXDIM_H


class sixdim
{
public:
    sixdim(int x,int y,double a1,double a2,
           double a3,double a4,double a5,double a6);
    double data[6];
    int x,y;
    bool operator<(const sixdim& sd)const;
    int getX();
    int getY();
};

#endif // SIXDIM_H
