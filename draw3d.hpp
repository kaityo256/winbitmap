#pragma once
#include "bm.hpp"
#include <algorithm>
#include <cmath>
#include <memory>

class draw3d {
private:
  struct Point {
    int X, Y;
    Point();
    Point(int _x, int _y) {
      X = _x;
      Y = _y;
    }
  };

  std::unique_ptr<winbitmap::canvas> canvas;
  double L;
  double Theta, Phi;
  double Sigma;
  int SX, SY;
  int width, height;
  double Magnification;

  Point get_point(double x, double y, double z) {
    x -= L * 0.5;
    y -= L * 0.5;
    z -= L * 0.5;
    double x1 = cos(Phi) * x + sin(Phi) * z;
    double y1 = y;
    double z1 = -sin(Phi) * x + cos(Phi) * z;

    double x2 = x1;
    double y2 = cos(Theta) * y1 + sin(Theta) * z1;
    x2 += L * 0.5;
    y2 += L * 0.5;
    x2 = x2 * Magnification + SX;
    y2 = y2 * Magnification + SY;
    return Point(static_cast<int>(x2), static_cast<int>(y2));
  }

  void draw_linee(double x1, double y1, double z1, double x2, double y2,
                  double z2) {
    Point p1 = get_point(x1, y1, z1);
    Point p2 = get_point(x2, y2, z2);
    canvas->moveto(p1.X, p1.Y);
    canvas->lineto(p2.X, p2.Y);
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
  struct vec3d {
    double x, y, z;
    vec3d(double _x, double _y, double _z) {
      x = _x;
      y = _y;
      z = _z;
    }
  };

  draw3d(double system_size, int image_size) {
    width = image_size;
    height = image_size;
    SX = static_cast<int>(width * 0.175);
    SY = static_cast<int>(height * 0.175);
    L = system_size;
    Theta = -0.45;
    Phi = 0.25;
    Sigma = 0.25;
    Magnification = static_cast<double>(width) / L * 0.625;
    canvas.reset(new winbitmap::canvas(width, height));
    canvas->set_color(255, 255, 255);
    canvas->fill_rect(0, 0, width, height);
  }

  void set_phi(double p) { Phi = p; }
  void set_theta(double t) { Theta = t; }
  void set_sigma(double s) { Sigma = s; }

  // void Draw(int n, double *qx, double *qy, double *qz) {
  void draw(std::vector<vec3d> &q) {
    std::vector<std::pair<int, double>> sorted_list;
    const int n = q.size();

    for (int i = 0; i < n; i++) {
      double x = q[i].x - L * 0.5;
      double y = q[i].y - L * 0.5;
      double z = q[i].z - L * 0.5;
      double y1 = y;
      double z1 = -sin(Phi) * x + cos(Phi) * z;
      double z2 = -sin(Theta) * y1 + cos(Theta) * z1;
      sorted_list.push_back(std::make_pair(i, z2));
    }
    std::sort(sorted_list.begin(), sorted_list.end(),
              [](const std::pair<int, double> &i, std::pair<int, double> &j) {
                return i.second < j.second;
              });

    draw_axis();
    const int r = static_cast<int>(Sigma * Magnification);
    for (int i = 0; i < n; i++) {
      Point p = get_point(q[i].x, 0, q[i].z);
      canvas->set_color(128, 128, 128);
      canvas->fill_circle(p.X, p.Y, r);
    }
    for (int i = 0; i < n; i++) {
      int j = sorted_list[i].first;
      draw3d::Point p = get_point(q[j].x, q[j].y, q[j].z);
      canvas->set_color(0, 0, 255);
      canvas->fill_circle(p.X, p.Y, r);
      canvas->set_color(0, 0, 0);
      canvas->draw_circle(p.X, p.Y, r);
    }
  }
  void save_to_file(const char *filename) { canvas->save_to_file(filename); };
};