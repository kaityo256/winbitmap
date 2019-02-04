/*
MIT License

Copyright (c) 2019 - present H. Watanabe

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

#include <fstream>
#include <iostream>
#include <vector>

namespace winbitmap {

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;

class canvas {
private:
  int width, height, line;
  std::vector<BYTE> image_buffer;
  int cx, cy;   // Current Point
  BYTE R, G, B; // Current Color

public:
  canvas(int w, int h) {
    width = w;
    height = h;
    line = ((w * 3 - 1) / 4) * 4 + 4;
    image_buffer.resize(line * h, 0);
    cx = 0;
    cy = 0;
  }

  void moveto(int x, int y) {
    cx = x;
    cy = y;
  }

  void lineto(int x, int y) {
    int dx = (x > cx) ? x - cx : cx - x;
    int dy = (y > cy) ? y - cy : cy - y;

    int sx = (x > cx) ? 1 : -1;
    int sy = (y > cy) ? 1 : -1;

    if (dx > dy) {
      int E = -dx;
      for (int i = 0; i <= dx; i++) {
        draw_point(cx, cy);
        cx += sx;
        E += 2 * dy;
        if (E >= 0) {
          cy += sy;
          E -= 2 * dx;
        }
      }
    } else {
      int E = -dy;
      for (int i = 0; i <= dy; i++) {
        draw_point(cx, cy);
        cy += sy;
        E += 2 * dx;
        if (E >= 0) {
          cx += sx;
          E -= 2 * dy;
        }
      }
    }
  }

  void draw_point(int x, int y) {
    if (x < 0 || x >= width)
      return;
    if (y < 0 || y >= height)
      return;
    int p = y * line + x * 3;
    image_buffer[p] = B;
    image_buffer[p + 1] = G;
    image_buffer[p + 2] = R;
  }

  void set_color(BYTE red, BYTE green, BYTE blue) {
    R = red;
    G = green;
    B = blue;
  }

  void draw_rect(int x, int y, int w, int h) {
    for (int iy = y; iy <= y + h; iy++) {
      draw_point(x, iy);
      draw_point(x + w, iy);
    }
    for (int ix = x; ix <= x + w; ix++) {
      draw_point(ix, y);
      draw_point(ix, y + h);
    }
  }

  void fill_rect(int x, int y, int w, int h) {
    for (int iy = 0; iy < h; iy++) {
      for (int ix = 0; ix < w; ix++) {
        draw_point(ix + x, iy + y);
      }
    }
  }

  void fill_circle(int x0, int y0, int r) {
    int x = r;
    int y = 0;
    int F = -2 * r + 3;
    while (x >= y) {
      for (int i = -x; i <= x; i++) {
        draw_point(x0 + i, y0 + y);
        draw_point(x0 + i, y0 - y);
        draw_point(x0 + y, y0 + i);
        draw_point(x0 - y, y0 + i);
      }
      if (F >= 0) {
        x--;
        F -= 4 * x;
      }
      y++;
      F += 4 * y + 2;
    }
  }

  void draw_circle(int x0, int y0, int r) {
    int x = r;
    int y = 0;
    int F = -2 * r + 3;
    while (x >= y) {
      draw_point(x0 + x, y0 + y);
      draw_point(x0 - x, y0 + y);
      draw_point(x0 + x, y0 - y);
      draw_point(x0 - x, y0 - y);
      draw_point(x0 + y, y0 + x);
      draw_point(x0 - y, y0 + x);
      draw_point(x0 + y, y0 - x);
      draw_point(x0 - y, y0 - x);
      if (F >= 0) {
        x--;
        F -= 4 * x;
      }
      y++;
      F += 4 * y + 2;
    }
  }

  void save_to_file(const char *filename) {
    DWORD bfSize = image_buffer.size() + 54;
    WORD bfReserved1 = 0;
    WORD bfReserved2 = 0;
    DWORD bfOffBits = 54;

    DWORD biSize = 40;
    DWORD biWidth = width;
    DWORD biHeight = height;
    WORD biPlanes = 1;
    WORD biBitCount = 24;
    DWORD biCompression = 0;
    DWORD biSizeImage = 0;
    DWORD biXPelsPerMeter = 0;
    DWORD biYPelsPerMeter = 0;
    DWORD biClrUsed = 0;
    DWORD biClrImportant = 0;

    std::ofstream fs(filename);
    // BITMAPFILEHEADER
    fs.write("BM", sizeof(WORD));
    fs.write((char *)&bfSize, sizeof(DWORD));
    fs.write((char *)&bfReserved1, sizeof(WORD));
    fs.write((char *)&bfReserved2, sizeof(WORD));
    fs.write((char *)&bfOffBits, sizeof(DWORD));
    // BITMAPINFOHEADER
    fs.write((char *)&biSize, sizeof(DWORD));
    fs.write((char *)&biWidth, sizeof(DWORD));
    fs.write((char *)&biHeight, sizeof(DWORD));
    fs.write((char *)&biPlanes, sizeof(WORD));
    fs.write((char *)&biBitCount, sizeof(WORD));
    fs.write((char *)&biCompression, sizeof(DWORD));
    fs.write((char *)&biSizeImage, sizeof(DWORD));
    fs.write((char *)&biXPelsPerMeter, sizeof(DWORD));
    fs.write((char *)&biYPelsPerMeter, sizeof(DWORD));
    fs.write((char *)&biClrUsed, sizeof(DWORD));
    fs.write((char *)&biClrImportant, sizeof(DWORD));
    // DATA
    fs.write((char *)image_buffer.data(), image_buffer.size());
    fs.close();
  }
};

} // namespace winbitmap
