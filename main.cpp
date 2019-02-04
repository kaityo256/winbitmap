#include "draw3d.hpp"
#include <cstdlib>
#include <iostream>

const int N = 10000;
double qx[N], qy[N], qz[N];

std::vector<draw3d::vec3d> atoms;

void add(int &n, double x, double y, double z) {
  qx[n] = x;
  qy[n] = y;
  qz[n] = z;
  atoms.push_back(draw3d::vec3d(x, y, z));
  n++;
}

int main(void) {
  const double L = 20.0;
  int n = 0;
  double cx = L * 0.5;
  double cy = L * 0.5;
  double cz = L * 0.5;
  double r = 6.0;
  double step = 1.1;
  double sh = step * 0.5;
  int is = static_cast<int>(r / step) + 1;
  for (int ix = -is; ix < is; ix++) {
    for (int iy = -is; iy < is; iy++) {
      for (int iz = -is; iz < is; iz++) {
        double x, y, z;
        x = ix * step;
        y = iy * step;
        z = iz * step;
        if (x * x + y * y + z * z < r * r)
          add(n, x + cx, y + cy, z + cz);
        x = ix * step + sh;
        y = iy * step + sh;
        z = iz * step;
        if (x * x + y * y + z * z < r * r)
          add(n, x + cx, y + cy, z + cz);
        x = ix * step + sh;
        y = iy * step;
        z = iz * step + sh;
        if (x * x + y * y + z * z < r * r)
          add(n, x + cx, y + cy, z + cz);
        x = ix * step;
        y = iy * step + sh;
        z = iz * step + sh;
        if (x * x + y * y + z * z < r * r)
          add(n, x + cx, y + cy, z + cz);
      }
    }
  }
  draw3d draw(L, 400);
  draw.draw(atoms);
  draw.save_to_file("test.bmp");
}
