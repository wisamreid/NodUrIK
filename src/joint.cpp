#include "joint.h"

using namespace stl;

Joint::Joint() : childLink(NULL), type(HINGE) {
  // Hinge Joint
  axes.push_back(Eigen::Vector3d(1,0,0));
  thetas.push_back(0.0);
}

Joint::Joint(Link* child_, JointType type_, double theta1, double theta2, double theta3) : childLink(child_), type(type_)  {
  switch (type)
  {
    case HINGE:
      axes.push_back(Eigen::Vector3d(1,0,0));
      thetas.push_back(theta1);
      break;
    case UNIVERSAL:
      axes.push_back(Eigen::Vector3d(1,0,0));
      axes.push_back(Eigen::Vector3d(0,1,0));
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      break;
    case BALL:
      axes.push_back(Eigen::Vector3d(1,0,0));
      axes.push_back(Eigen::Vector3d(0,1,0));
      axes.push_back(Eigen::Vector3d(0,0,1));
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      thetas.push_back(theta3);
      break;
    case SLIDER:
      axes.push_back(Eigen::Vector3d(0,0,1));
      thetas.push_back(theta1);
      break;
  }
}

void Joint::draw() {
  //Assume that some parent link updated the OpenGL matrix stack. Under that assumption, draw this joint.

  //Based on the thetas, rotate around the axes of rotions and update hte OpenGL transform stack based on the new rotation.
  switch (type)
  {
    case HINGE:
      // Draw Hinge Here (Cylinder)
      break;
    case UNIVERSAL:
      // Draw Universal Here (Sphere)
      break;
    case BALL:
      // Draw Ball Here (Sphere)
      break;
    case SLIDER:
      // Draw Slider Here (Cylinder)
      break;
  }

  if (childLink) childLink->draw();

}

int Joint::GetNumDOFS() {
  switch (type)
  {
    case HINGE:
      return 1;
      break;
    case UNIVERSAL:
      return 2;
      break;
    case BALL:
      return 3;
      break;
    case SLIDER:
      return 1;
      break;
  }
}

void Joint::SetDOFS(Dofs& dofs, int& startIndex) {
  switch (type)
  {
    case HINGE:
      thetas[0] = dofs[startIndex++];
      break;
    case UNIVERSAL:
      thetas[0] = dofs[startIndex++];
      thetas[1] = dofs[startIndex++];
      break;
    case BALL:
      thetas[0] = dofs[startIndex++];
      thetas[1] = dofs[startIndex++];
      thetas[2] = dofs[startIndex++];
      break;
    case SLIDER:
      thetas[0] = dofs[startIndex++];
      break;
  }
}
