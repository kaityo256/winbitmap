# winbitmap

[![MIT License](http://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

A single file C++ library to generate Microsoft Windows Bitmap files with minimal dependencies. It depends only on STL.

## Usage

Just include a header file `winbitmap.hpp`. Then you can use `winbitmap::canvas` class. Here is an example.

```cpp
#include "winbitmap.hpp"

int main() {
  // Prepare a canvas (width, height)
  winbitmap::canvas canvas(256, 256);

  // Draw a filled rectangle
  canvas.set_color(255, 255, 255);
  canvas.fill_rect(0, 0, 256, 256);

  // Draw a filled circle
  canvas.set_color(255, 0, 0);
  canvas.fill_circle(128, 128, 32);

  // Draw a line
  canvas.set_color(0, 0, 0);
  canvas.moveto(0, 0);
  canvas.lineto(256, 256);

  // Draw a rectangle
  canvas.set_color(0, 0, 255);
  canvas.draw_rect(32, 32, 64, 32);

  canvas.save_to_file("sample.bmp");
  std::cout << "Saved to sample.bmp" << std::endl;
}
```

Then compile and execute the above.

```sh
$ g++ test.cpp
$ ./a.out
Saved to sample.bmp
```

Then you will have the following image.

![fig/test.png](fig/test.png)

## Another Example

You can find another example in `sample` folder.

```sh
$ cd sample
$ make
$ ./a.out
Saved to test3d.bmp
```

You will have the following image.

![fig/test3d.png](fig/test3d.png)

## License

This software is released under the MIT License, see LICENSE.