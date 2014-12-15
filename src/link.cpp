#include "link.h"

using namespace stl;

Link::Link(const double length_, const double width_, Joint* child_) : length(length_), width(width_), childJoint(child_) {}

void Link::draw() {
  glutSolidCone(width, length, 20, 20);
  if (childJoint) {
    glTranslatef(0,0,length);
    childJoint->draw();
  }
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
