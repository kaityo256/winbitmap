#include "draw3d.hpp"

int main(void) {
  std::vector<draw3d::vec3d> atoms;
  const double L = 20.0;
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
          atoms.push_back(draw3d::vec3d(x + cx, y + cy, z + cz));
        x = ix * step + sh;
        y = iy * step + sh;
        z = iz * step;
        if (x * x + y * y + z * z < r * r)
          atoms.push_back(draw3d::vec3d(x + cx, y + cy, z + cz));
        x = ix * step + sh;
        y = iy * step;
        z = iz * step + sh;
        if (x * x + y * y + z * z < r * r)
          atoms.push_back(draw3d::vec3d(x + cx, y + cy, z + cz));
        x = ix * step;
        y = iy * step + sh;
        z = iz * step + sh;
        if (x * x + y * y + z * z < r * r)
          atoms.push_back(draw3d::vec3d(x + cx, y + cy, z + cz));
      }
    }
  }
  draw3d::image img(L, 400);
  img.draw(atoms);
  img.save_to_file("test.bmp");
}
