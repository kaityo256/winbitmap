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
  int Width, Height, Line;
  std::vector<BYTE> ImageBuffer;
  int CX, CY; // Current Point
  BYTE R, G, B;

public:
  canvas(int w, int h) {
    Width = w;
    Height = h;
    Line = ((w * 3 - 1) / 4) * 4 + 4;
    ImageBuffer.resize(Line * h, 0);
    CX = 0;
    CY = 0;
  }

  void MoveTo(int x, int y) {
    CX = x;
    CY = y;
  }

  void LineTo(int x, int y) {
    int dx = (x > CX) ? x - CX : CX - x;
    int dy = (y > CY) ? y - CY : CY - y;

    int sx = (x > CX) ? 1 : -1;
    int sy = (y > CY) ? 1 : -1;

    if (dx > dy) {
      int E = -dx;
      for (int i = 0; i <= dx; i++) {
        DrawPoint(CX, CY);
        CX += sx;
        E += 2 * dy;
        if (E >= 0) {
          CY += sy;
          E -= 2 * dx;
        }
      }
    } else {
      int E = -dy;
      for (int i = 0; i <= dy; i++) {
        DrawPoint(CX, CY);
        CY += sy;
        E += 2 * dx;
        if (E >= 0) {
          CX += sx;
          E -= 2 * dy;
        }
      }
    }
  }

  void DrawPoint(int x, int y) {
    if (x < 0 || x >= Width)
      return;
    if (y < 0 || y >= Height)
      return;
    int p = y * Line + x * 3;
    ImageBuffer[p] = B;
    ImageBuffer[p + 1] = G;
    ImageBuffer[p + 2] = R;
  }

  void SetColor(BYTE red, BYTE green, BYTE blue) {
    R = red;
    G = green;
    B = blue;
  }

  void FillRect(int x, int y, int w, int h) {
    for (int iy = 0; iy < h; iy++) {
      for (int ix = 0; ix < w; ix++) {
        DrawPoint(ix + x, iy + y);
      }
    }
  }

  void FillCircle(int x0, int y0, int r) {
    int x = r;
    int y = 0;
    int F = -2 * r + 3;
    while (x >= y) {
      for (int i = -x; i <= x; i++) {
        DrawPoint(x0 + i, y0 + y);
        DrawPoint(x0 + i, y0 - y);
        DrawPoint(x0 + y, y0 + i);
        DrawPoint(x0 - y, y0 + i);
      }
      if (F >= 0) {
        x--;
        F -= 4 * x;
      }
      y++;
      F += 4 * y + 2;
    }
  }

  void DrawCircle(int x0, int y0, int r) {
    int x = r;
    int y = 0;
    int F = -2 * r + 3;
    while (x >= y) {
      DrawPoint(x0 + x, y0 + y);
      DrawPoint(x0 - x, y0 + y);
      DrawPoint(x0 + x, y0 - y);
      DrawPoint(x0 - x, y0 - y);
      DrawPoint(x0 + y, y0 + x);
      DrawPoint(x0 - y, y0 + x);
      DrawPoint(x0 + y, y0 - x);
      DrawPoint(x0 - y, y0 - x);
      if (F >= 0) {
        x--;
        F -= 4 * x;
      }
      y++;
      F += 4 * y + 2;
    }
  }

  void SaveToFile(const char *filename) {
    DWORD bfSize = ImageBuffer.size() + 54;
    WORD bfReserved1 = 0;
    WORD bfReserved2 = 0;
    DWORD bfOffBits = 54;

    DWORD biSize = 40;
    DWORD biWidth = Width;
    DWORD biHeight = Height;
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
    fs.write((char *)ImageBuffer.data(), ImageBuffer.size());
    fs.close();
  }
};

} // namespace winbitmap