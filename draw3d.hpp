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
#ifndef draw3d_h
#define draw3d_h
#include "bm.h"
//----------------------------------------------------------------------
struct Point{
  int X, Y; 
  Point();
  Point(int _x, int _y){
    X = _x;Y = _y;
  };
};
//----------------------------------------------------------------------
class Draw3D{
  private:
    WindowsBitmap *canvas;
    int particleNumber;
    double L;
    double Theta, Phi;
    double Sigma;
    static double *depth;
    int *sorted_index;
    int SX, SY;
    int Width, Height;
    double Magnification;
    Point GetPoint(double x, double y, double z);
    void DrawLine(double x1, double y1, double z1,double x2, double y2, double z2);
    void DrawAxis(void);
    void SortParticles(void);
  public:
    Draw3D(double ss, int is);
    ~Draw3D(void);
    void SetPhi(double p){Phi = p;};
    void SetTheta(double t){Theta = t;};
    void SetSigma(double s){Sigma = s;};
    void Draw(int n, double *qx, double *qy, double *qz);
    void SaveToFile(const char *filename){canvas->SaveToFile(filename);};
    static bool Compare(int i, int j);
};
//----------------------------------------------------------------------
#endif
