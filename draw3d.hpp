#pragma once
#include "bm.hpp"

struct Point {
  int X, Y;
  Point();
  Point(int _x, int _y) {
    X = _x;
    Y = _y;
  }
};

class Draw3D {
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
  void DrawLine(double x1, double y1, double z1, double x2, double y2,
                double z2);
  void DrawAxis(void);
  void SortParticles(void);

public:
  Draw3D(double ss, int is);
  ~Draw3D(void);
  void SetPhi(double p) { Phi = p; };
  void SetTheta(double t) { Theta = t; };
  void SetSigma(double s) { Sigma = s; };
  void Draw(int n, double *qx, double *qy, double *qz);
  void SaveToFile(const char *filename) { canvas->SaveToFile(filename); };
  static bool Compare(int i, int j);
};