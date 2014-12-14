#include "viewport.h"
#include <iostream>

using namespace stl;

Viewport::Viewport(): w(400), h(400) {}
Viewport::Viewport(int w_, int h_): w(w_), h(h_) {}
// Viewport::Viewport(): w(400), h(400), imageEnabled(false) {imageBuffer.resize(w*h);}
// Viewport::Viewport(int w_, int h_): w(w_), h(h_), imageEnabled(false) {imageBuffer.resize(w*h);}
const int Viewport::getH() const {return h;}
const int Viewport::getW() const {return w;}

// //****************************************************
// // A routine to set a pixel by drawing a GL point.  This is not a
// // general purpose routine as it assumes a lot of stuff specific to
// // this example.
// //****************************************************
// void Viewport::setPixel(int x, int y, const Color& c) {
//   glColor3f(c.getR(), c.getG(), c.getB());
//   glVertex2f(x + 0.5, y + 0.5);   // The 0.5 is to target pixel
//   // centers 
//   // Note: Need to check for gap
//   // bug on inst machines.

//   if (imageEnabled) {
//     double r = std::min(1.0,std::max(0.0,c.getR()));
//     double g = std::min(1.0,std::max(0.0,c.getG()));
//     double b = std::min(1.0,std::max(0.0,c.getB()));
//     imageBuffer[(h-1-y)*w+x] = (int)(r*255+0.5)|(int)(g*255+0.5)<<8|(int)(b*255+0.5)<<16|255<<24;
//   }
// }

// void Viewport::beginImage() {
//   imageEnabled = true;
//   float rgba[4] = {-1,-1,-1,-1};
//   glGetFloatv(GL_COLOR_CLEAR_VALUE, rgba);
//   std::fill(imageBuffer.begin(), imageBuffer.end(), (int)(rgba[0]*255+0.5)|(int)(rgba[1]*255+0.5)<<8|(int)(rgba[2]*255+0.5)<<16|255<<24);
// }

// void Viewport::endImage() {
//   imageEnabled = false;
// }

void Viewport::resize(int w_, int h_) {
  w = w_;
  h = h_;
  // imageBuffer.clear();
  // imageBuffer.resize(w*h);
  // imageEnabled = false;
}

// void Viewport::writeImage(const char* filename)
// {
//   //Save the image
//   unsigned error = lodepng::encode(filename, (unsigned char*)&imageBuffer[0], w, h);

//   if (error) {
//     std::cout << "PNG error " << error << ": "<< lodepng_error_text(error) << std::endl;
//   }
// }
