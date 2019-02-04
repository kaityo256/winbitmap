#pragma once
#include "bm.hpp"
#include <algorithm>
#include <cmath>
#include <memory>

namespace draw3d {
struct vec3d {
  double x, y, z;
  vec3d(double _x, double _y, double _z) {
    x = _x;
    y = _y;
    z = _z;
  }
};

struct vec2di {
  int x, y;
  vec2di();
  vec2di(int _x, int _y) {
    x = _x;
    y = _y;
  }
};

class image {
private:
  std::unique_ptr<winbitmap::canvas> canvas;
  double L;
  double theta, phi;
  double sigma;
  int margin_x, margin_y;
  int width, height;
  double Magnification;

  vec2di get_point(double x, double y, double z) {
    x -= L * 0.5;
    y -= L * 0.5;
    z -= L * 0.5;
    double x1 = cos(phi) * x + sin(phi) * z;
    double y1 = y;
    double z1 = -sin(phi) * x + cos(phi) * z;

    double x2 = x1;
    double y2 = cos(theta) * y1 + sin(theta) * z1;
    x2 += L * 0.5;
    y2 += L * 0.5;
    x2 = x2 * Magnification + margin_x;
    y2 = y2 * Magnification + margin_y;
    return vec2di(static_cast<int>(x2), static_cast<int>(y2));
  }

  void draw_linee(double x1, double y1, double z1, double x2, double y2,
                  double z2) {
    vec2di p1 = get_point(x1, y1, z1);
    vec2di p2 = get_point(x2, y2, z2);
    canvas->moveto(p1.x, p1.y);
    canvas->lineto(p2.x, p2.y);
  }

  void draw_axis(void) {
    canvas->set_color(128, 128, 128);
    draw_linee(0, 0, 0, 0, 0, L);
    draw_linee(0, 0, 0, 0, L, 0);
    draw_linee(0, 0, 0, L, 0, 0);

    draw_linee(L, 0, 0, L, L, 0);
    draw_linee(L, 0, 0, L, 0, L);

    draw_linee(0, L, 0, 0, L, L);
    draw_linee(0, L, 0, L, L, 0);

    draw_linee(0, 0, L, L, 0, L);
    draw_linee(0, 0, L, 0, L, L);

    draw_linee(0, L, L, L, L, L);
    draw_linee(L, L, 0, L, L, L);
    draw_linee(L, 0, L, L, L, L);
  }

public:
  image(double system_size, int image_size) {
    width = image_size;
    height = image_size;
    margin_x = static_cast<int>(width * 0.175);
    margin_y = static_cast<int>(height * 0.175);
    L = system_size;
    theta = -0.45;
    phi = 0.25;
    sigma = 0.25;
    Magnification = static_cast<double>(width) / L * 0.625;
    canvas.reset(new winbitmap::canvas(width, height));
    canvas->set_color(255, 255, 255);
    canvas->fill_rect(0, 0, width, height);
  }

  void set_phi(double p) { phi = p; }
  void set_theta(double t) { theta = t; }
  void set_sigma(double s) { sigma = s; }

  // void Draw(int n, double *qx, double *qy, double *qz) {
  void draw(std::vector<vec3d> &q) {
    std::vector<std::pair<int, double>> sorted_list;
    const int n = q.size();

    for (int i = 0; i < n; i++) {
      double x = q[i].x - L * 0.5;
      double y = q[i].y - L * 0.5;
      double z = q[i].z - L * 0.5;
      double y1 = y;
      double z1 = -sin(phi) * x + cos(phi) * z;
      double z2 = -sin(theta) * y1 + cos(theta) * z1;
      sorted_list.push_back(std::make_pair(i, z2));
    }
    std::sort(sorted_list.begin(), sorted_list.end(),
              [](const std::pair<int, double> &i, std::pair<int, double> &j) {
                return i.second < j.second;
              });

    draw_axis();
    const int r = static_cast<int>(sigma * Magnification);
    for (int i = 0; i < n; i++) {
      vec2di p = get_point(q[i].x, 0, q[i].z);
      canvas->set_color(128, 128, 128);
      canvas->fill_circle(p.x, p.y, r);
    }
    for (int i = 0; i < n; i++) {
      int j = sorted_list[i].first;
      vec2di p = get_point(q[j].x, q[j].y, q[j].z);
      canvas->set_color(0, 0, 255);
      canvas->fill_circle(p.x, p.y, r);
      canvas->set_color(0, 0, 0);
      canvas->draw_circle(p.x, p.y, r);
    }
  }
  void save_to_file(const char *filename) { canvas->save_to_file(filename); };
};

} // namespace draw3d