#include "link.h"

using namespace stl;

Link::Link(const double width_, const double length_, Joint* child_) : width(width_), length(length_), childJoint(child_) {}

void Link::draw() {
  //Even if a parent joint doesn't exist, assume that some parent joint updated the OpenGL matrix stack. Under that assumption, draw this link.

  //Push the following translation to the OpenGL stack: (0,0,length)
  //Why am i using (0,0,length)? Because i'm assuming all links point in the +z direction.
  if (childJoint) childJoint->draw();

    glutSolidCone(width, length, 20, 20);


}

void Link::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  currEndEffector = (globalTransform * Eigen::Translation3d(0,0,length)).translation();
}

void Link::UpdateTransform(Transform3d& currGlobalTransform) {
  globalTransform = currGlobalTransform;
  if (childJoint) {
    Transform3d tempTransform = globalTransform * Eigen::Translation3d(0,0,length);
    childJoint->UpdateTransform(tempTransform);
  }
}
