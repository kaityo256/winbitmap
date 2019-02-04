#include <iostream>
#include <cstdlib>
#include "draw3d.hpp"

const int N = 10000;
double qx[N],qy[N],qz[N];

void add(int &n,double x, double y, double z){
  qx[n] = x;
  qy[n] = y;
  qz[n] = z;
  n++;
}

int main(void){
  const double L = 20.0;
  int n = 0;
  double cx = L*0.5;
  double cy = L*0.5;
  double cz = L*0.5;
  double r = 6.0; 
  double step = 1.1;
  double sh = step*0.5;
  int is = static_cast<int>(r/step)+1;
  for(int ix=-is;ix<is;ix++){
    for(int iy=-is;iy<is;iy++){
      for(int iz=-is;iz<is;iz++){
        double x,y,z;
        x = ix*step;
        y = iy*step;
        z = iz*step;
        if(x*x+y*y+z*z < r*r) add(n,x+cx,y+cy,z+cz);
        x = ix*step+sh;
        y = iy*step+sh;
        z = iz*step;
        if(x*x+y*y+z*z < r*r) add(n,x+cx,y+cy,z+cz);
        x = ix*step+sh;
        y = iy*step;
        z = iz*step+sh;
        if(x*x+y*y+z*z < r*r) add(n,x+cx,y+cy,z+cz);
        x = ix*step;
        y = iy*step+sh;
        z = iz*step+sh;
        if(x*x+y*y+z*z < r*r) add(n,x+cx,y+cy,z+cz);
      }
    }
  } 
  Draw3D *draw = new Draw3D(L,400);
  draw->Draw(n,qx,qy,qz);
  draw->SaveToFile("test.bmp");
  delete draw;
}
