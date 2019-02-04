/*
  Copyright (C) 2013, Hiroshi Watanabe
  All rights reserved.

  Redistribution and use in source and binary forms,
  with or without modification, are permitted provided
  that the following conditions are met:

    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials
      provided with the distribution.
    * Neither the name of <organization> nor the names of its
      contributors may be used to endorse or promote products derived
      from this software without specific prior written permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
  HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
  SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
  LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
  DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
  THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
  (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/
#include <iostream>
#include <stdlib.h>
#include "draw3d.h"
//----------------------------------------------------------------------
const int N = 10000;
double qx[N],qy[N],qz[N];
//----------------------------------------------------------------------
double
myrand(void){
  return (double)rand()/(double)RAND_MAX;
}
//----------------------------------------------------------------------
void
add(int &n,double x, double y, double z){
  qx[n] = x;
  qy[n] = y;
  qz[n] = z;
  n++;
}
//----------------------------------------------------------------------
int
main(void){
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
//----------------------------------------------------------------------
