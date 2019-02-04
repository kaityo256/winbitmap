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
#include <math.h>
#include <algorithm>
#include "draw3d.h"
using namespace std;

double *Draw3D::depth;
//----------------------------------------------------------------------
Draw3D::Draw3D(double system_size, int image_size){
  Width  = image_size;
  Height = image_size;
  SX = static_cast<int>(Width*0.175);
  SY = static_cast<int>(Height*0.175);
  L = system_size;
  Theta = -0.45;
  Phi = 0.25;
  Sigma = 0.25;
  Magnification = static_cast<double>(Width)/L*0.625;
  canvas = new WindowsBitmap(Width,Height);
  canvas->SetColor(255,255,255);
  canvas->FillRect(0,0,Width,Height);
}
//----------------------------------------------------------------------
Draw3D::~Draw3D(void){
  delete canvas;
}
//----------------------------------------------------------------------
bool
Draw3D::Compare(const int i, const int j){
  return depth[i] < depth[j];
}
//----------------------------------------------------------------------
void
Draw3D::Draw(int n, double *qx, double *qy, double *qz){
  particleNumber = n;
  sorted_index = new int[n];
  depth = new double[n];

  for(int i=0;i<n;i++){
    sorted_index[i] = i;
  }
  for(int i=0;i<n;i++){
    double x = qx[i] - L*0.5;
    double y = qy[i] - L*0.5;
    double z = qz[i] - L*0.5;
    double y1 = y;
    double z1 = -sin(Phi)*x + cos(Phi)*z;
    double z2 = -sin(Theta)*y1 + cos(Theta)*z1;
    depth[i] = z2;
  }
  sort(sorted_index,sorted_index + n,Compare);  

  DrawAxis();
  const int r = static_cast<int>(Sigma*Magnification);
  for(int i=0;i<n;i++){
    Point p = GetPoint(qx[i],0,qz[i]);
    canvas->SetColor(128,128,128);
    canvas->FillCircle(p.X,p.Y,r);
  }
  for(int i=0;i<n;i++){
    int j = sorted_index[i];
    Point p = GetPoint(qx[j],qy[j],qz[j]);
    canvas->SetColor(0,0,255);
    canvas->FillCircle(p.X,p.Y,r);
    canvas->SetColor(0,0,0);
    canvas->DrawCircle(p.X,p.Y,r);
  }
  delete [] sorted_index;
  delete [] depth;
}
//----------------------------------------------------------------------
Point
Draw3D::GetPoint(double x, double y, double z){
  x -= L*0.5;
  y -= L*0.5;
  z -= L*0.5;
  double x1 = cos(Phi)*x + sin(Phi)*z;
  double y1 = y;
  double z1 = -sin(Phi)*x + cos(Phi)*z; 

  double x2 = x1;
  double y2 = cos(Theta)*y1+sin(Theta)*z1;
  x2 += L*0.5;
  y2 += L*0.5;
  x2 = x2*Magnification + SX;
  y2 = y2*Magnification + SY;
  return Point(static_cast<int>(x2),static_cast<int>(y2));
}
//----------------------------------------------------------------------
void
Draw3D::DrawLine(double x1, double y1, double z1,double x2, double y2, double z2){
  Point p1 = GetPoint(x1,y1,z1);
  Point p2 = GetPoint(x2,y2,z2);
  canvas->MoveTo(p1.X,p1.Y);
  canvas->LineTo(p2.X,p2.Y);
}
//----------------------------------------------------------------------
void
Draw3D::DrawAxis(void){
  canvas->SetColor(128,128,128);
  DrawLine(0,0,0,0,0,L);
  DrawLine(0,0,0,0,L,0);
  DrawLine(0,0,0,L,0,0);

  DrawLine(L,0,0,L,L,0);
  DrawLine(L,0,0,L,0,L);

  DrawLine(0,L,0,0,L,L);
  DrawLine(0,L,0,L,L,0);

  DrawLine(0,0,L,L,0,L);
  DrawLine(0,0,L,0,L,L);

  DrawLine(0,L,L,L,L,L);
  DrawLine(L,L,0,L,L,L);
  DrawLine(L,0,L,L,L,L);

}
//----------------------------------------------------------------------
