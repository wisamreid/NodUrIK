#include "viewport.h"

using namespace stl;

Viewport::Viewport(): w(400), h(400) {}
Viewport::Viewport(int w_, int h_): w(w_), h(h_) {}
const int Viewport::getH() const {return h;}
const int Viewport::getW() const {return w;}

void Viewport::resize(int w_, int h_) {
  w = w_;
  h = h_;
}

