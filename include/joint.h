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

  enum JointType {
    UNIVERSAL,
    SOMETHING,
    PISTON
  };

  class Link;

  class Joint {
  private:
    Link* childLink;
    Eigen::Vector3d anchor; // Relative to the tip of the parent link
    std::vector<Eigen::Vector3d> axes; // Relative to the tip of the parent link
    //double* thetas;
    JointType type;

  public:
    Joint();
    Joint(Link* child_, JointType type_);

    void draw();
  };

}

#endif
