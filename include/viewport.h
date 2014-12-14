#ifndef VIEWPORT_H
#define VIEWPORT_H

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

// #include "../lodepng/lodepng.h"
// #include <vector>
// #include "color.h"

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

class Viewport {
 private:
  int w, h; // width and height

 //  bool imageEnabled;
 //  std::vector<int> imageBuffer;

 public:
  Viewport();
  Viewport(int w_, int h_);
  const int getH() const;
  const int getW() const;
 //  void setPixel(int x, int y, const Color& c);
 //  void beginImage();
 //  void endImage();
  void resize(int w_, int h_);
 //  void writeImage(const char* filename);
};

}

#endif
