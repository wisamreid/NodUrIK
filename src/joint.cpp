#include "joint.h"

using namespace stl;

Joint::Joint() : childLink(NULL), type(UNIVERSAL) {}
Joint::Joint(Link* child_, JointType type_) : childLink(child_), type(type_)  {}

void Joint::draw() {
  //Assume that some parent link updated the OpenGL matrix stack. Under that assumption, draw this joint.

  //Assuming you have the Thetas, (I will add them in late, but for now feel free to hard code thetas in this function), use those thetas to rotate around the axes of rotions and update hte OpenGL transform stack based on the new rotion.

  if (childLink) childLink->draw();
}