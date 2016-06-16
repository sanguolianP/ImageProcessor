#include "sixdim.h"

sixdim::sixdim(int x,int y,double a1,double a2,
               double a3,double a4,double a5,double a6)
    :x(x),y(y)
{
    data[0]=a1;data[1]=a2;
    data[2]=a3;data[3]=a4;
    data[4]=a5;data[5]=a6;
}

bool sixdim::operator <(const sixdim& sd) const
{
    for(int i=0;i<6;i++)
    {
        if(data[i]<sd.data[i]) return true;
        if(data[i]>sd.data[i]) return false;
    }
    return false;
}

int sixdim::getX()
{
    return x;
}

int sixdim::getY()
{
    return y;
}
