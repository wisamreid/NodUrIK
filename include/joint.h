#ifndef JOINT_H
#define JOINT_H

#include <iostream>
#include <vector>
#include <Eigen/Core>
#include "link.h"

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

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  typedef Eigen::Matrix<double,1,Dynamic> Dofs;
  typedef Eigen::Matrix<double,3,Dynamic> Jacobian;

  enum JointType {
    BALL,         // 3 axes of rotation
    UNIVERSAL,    // 2 axes of rotation
    HINGE,        // 1 axis of rotation
    SLIDER        // 0 axis of rotation, 1 axis of translation
  };

  class Link;

  class Joint {
  private:
    Link* childLink;
    Eigen::Vector3d anchor; // Relative to the tip of the parent link
    std::vector<Eigen::Vector3d> axes; // Relative to the tip of the parent link
    std::vector<double> thetas;
    JointType type;

  public:
    Joint();
    Joint(Link* child_, JointType type_);

    void draw();
    int GetNumDOFS();
    void SetDOFS(Dofs& dofs, int& startIndex);
  };

}

#endif
