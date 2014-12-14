#include "link.h"

using namespace stl;

Link::Link(const double length_, Joint* child_) : length(length_), childJoint(child_) {}

void Link::draw() {
  //Even if a parent joint doesn't exist, assume that some parent joint updated the OpenGL matrix stack. Under that assumption, draw this link.

  //Push the following translation to the OpenGL stack: (0,0,length)
  //Why am i using (0,0,length)? Because i'm assuming all links point in the +z direction.
  if (childJoint) childJoint->draw();
}
