#include "joint.h"

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

using namespace stl;

Joint::Joint(Link* child_, JointType type_, double theta1, double theta2, double theta3, double size_) : childLink(child_), type(type_), size(size_)  {
  switch (type)
  {
    case HINGE:
      thetas.push_back(theta1);
      break;
    case UNIVERSAL:
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      break;
    case BALL:
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      thetas.push_back(theta3);
      break;
    case SLIDER:
      thetas.push_back(theta1);
      break;
  }
}

void Joint::draw() {
  //Assume that some parent link updated the OpenGL matrix stack. Under that assumption, draw this joint.

  //Based on the thetas, rotate around the axes of rotions and update hte OpenGL transform stack based on the new rotation.
  switch (type)
  {
    case HINGE: {
      // Draw Hinge Here (Cylinder)
      // glutSolidCylinder(size, size, 20, 20);
      GLUquadricObj *hinge = gluNewQuadric();
      gluCylinder(hinge, size, size, size, 30, 30);
    }
      break;
    case UNIVERSAL: {
      // Draw Universal Here (Sphere)
      glutSolidSphere(size, 20, 20);
    }
      break;
    case BALL: {
      // Draw Ball Here (Sphere)
      glutSolidSphere(size, 20, 20);
    }
      break;
    case SLIDER: {
      // Draw Slider Here (Cylinder)
    }
      break;
  }

  if (childLink) childLink->draw();

}

void Joint::UpdateTransform(Transform3d& currGlobalTransform) {
  globalTransform = currGlobalTransform;
  if (childLink) {
    Transform3d tempTransform;
    switch (type)
    {
      case HINGE:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX());
        tempTransform = globalTransform * Eigen::Translation3d(Eigen::Vector3d::UnitZ()*size);
        break;
      case UNIVERSAL:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(thetas[1], Eigen::Vector3d::UnitY());
        tempTransform = globalTransform * Eigen::Translation3d(Eigen::Vector3d::UnitZ()*size);
        break;
      case BALL:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(thetas[1], Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(thetas[2], Eigen::Vector3d::UnitZ());
        tempTransform = globalTransform * Eigen::Translation3d(Eigen::Vector3d::UnitZ()*size);
        break;
      case SLIDER:
        tempTransform = globalTransform * Eigen::Translation3d(Eigen::Vector3d::UnitZ()*thetas[0]);
        break;
    }
    childLink->UpdateTransform(tempTransform);
  }
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
