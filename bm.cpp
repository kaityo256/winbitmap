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
//----------------------------------------------------------------------
#include <iostream>
#include <fstream>
#include "bm.h"
using namespace std;
//----------------------------------------------------------------------
WindowsBitmap::WindowsBitmap(int w, int h){
  Width = w;
  Height = h;
  Line = ((w*3 -1)/4)*4 + 4; 
  BufferSize = Line*h;
  ImageBuffer = new BYTE[BufferSize];
  for(int i=0;i<BufferSize;i++){
    ImageBuffer[i] = 0;
  }
  CX = 0;
  CY = 0;
};
//----------------------------------------------------------------------
void
WindowsBitmap::DrawPoint(int x, int y){
  if(x <0 || x >= Width)return;
  if(y <0 || y >= Height)return;
  int p = y*Line+x*3;
  ImageBuffer[p] = B;
  ImageBuffer[p+1] = G;
  ImageBuffer[p+2] = R;
}
//----------------------------------------------------------------------
void
WindowsBitmap::LineTo(int x, int y){
  int dx = ( x > CX ) ? x - CX : CX - x;
  int dy = ( y > CY ) ? y - CY : CY - y;

  int sx = ( x > CX ) ? 1 : -1;
  int sy = ( y > CY ) ? 1 : -1;

  if ( dx > dy ) {
    int E = -dx;
    for (int i = 0 ; i <= dx ; i++ ) {
      DrawPoint(CX,CY);
      CX += sx;
      E += 2 * dy;
      if ( E >= 0 ) {
        CY += sy;
        E -= 2 * dx;
      }
    }
  } else {
    int E = -dy;
    for (int i = 0 ; i <= dy ; i++ ) {
      DrawPoint(CX,CY);
      CY += sy;
      E += 2 * dx;
      if ( E >= 0 ) {
        CX += sx;
        E -= 2 * dy;
      }
    }
  }

}
//----------------------------------------------------------------------
void
WindowsBitmap::FillRect(int x, int y, int w, int h){
  for(int iy=0;iy<h;iy++){
    for(int ix=0;ix<w;ix++){
      DrawPoint(ix + x, iy + y);
    }
  }
}
//----------------------------------------------------------------------
void 
WindowsBitmap::DrawCircle(int x0, int y0, int r){
  int x = r;
  int y = 0;
  int F = -2 * r + 3;
  while ( x >= y ) {
    DrawPoint( x0 + x, y0 + y);
    DrawPoint( x0 - x, y0 + y);
    DrawPoint( x0 + x, y0 - y);
    DrawPoint( x0 - x, y0 - y);
    DrawPoint( x0 + y, y0 + x);
    DrawPoint( x0 - y, y0 + x);
    DrawPoint( x0 + y, y0 - x);
    DrawPoint( x0 - y, y0 - x);
    if ( F >= 0 ) {
      x--;
      F -= 4 * x;
    }
    y++;
    F += 4 * y + 2;
  }
}
//----------------------------------------------------------------------
void 
WindowsBitmap::FillCircle(int x0, int y0, int r){
  int x = r;
  int y = 0;
  int F = -2 * r + 3;
  while ( x >= y ) {
    for(int i=-x;i<=x;i++){
      DrawPoint( x0 + i, y0 + y);
      DrawPoint( x0 + i, y0 - y);
      DrawPoint( x0 + y, y0 + i);
      DrawPoint( x0 - y, y0 + i);
    }
    if ( F >= 0 ) {
      x--;
      F -= 4 * x;
    }
    y++;
    F += 4 * y + 2;
  }
}
//----------------------------------------------------------------------
void
WindowsBitmap::SetColor(BYTE red,BYTE green,BYTE blue){
  R = red;
  G = green;
  B = blue;
}
//----------------------------------------------------------------------
void
WindowsBitmap::SaveToFile(const char * filename){
  DWORD bfSize = BufferSize+54;
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
  DWORD biClrImportant= 0;

  ofstream fs(filename);
  //BITMAPFILEHEADER
  fs.write("BM",sizeof(WORD));
  fs.write((char *)&bfSize,sizeof(DWORD));
  fs.write((char *)&bfReserved1,sizeof(WORD));
  fs.write((char *)&bfReserved2,sizeof(WORD));
  fs.write((char *)&bfOffBits,sizeof(DWORD));
  //BITMAPINFOHEADER
  fs.write((char *)&biSize,sizeof(DWORD));
  fs.write((char *)&biWidth,sizeof(DWORD));
  fs.write((char *)&biHeight,sizeof(DWORD));
  fs.write((char *)&biPlanes,sizeof(WORD));
  fs.write((char *)&biBitCount,sizeof(WORD));
  fs.write((char *)&biCompression,sizeof(DWORD));
  fs.write((char *)&biSizeImage,sizeof(DWORD));
  fs.write((char *)&biXPelsPerMeter,sizeof(DWORD));
  fs.write((char *)&biYPelsPerMeter,sizeof(DWORD));
  fs.write((char *)&biClrUsed,sizeof(DWORD));
  fs.write((char *)&biClrImportant,sizeof(DWORD));
  //DATA
  fs.write((char *)ImageBuffer,BufferSize);
  fs.close();
}
//----------------------------------------------------------------------
