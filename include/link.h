#ifndef LINK_H
#define LINK_H

#include <Eigen/Core>
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

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  typedef Eigen::Matrix<double,1,Eigen::Dynamic> Dofs;
  typedef Eigen::Matrix<double,3,Eigen::Dynamic> Jacobian;
  typedef Eigen::Transform<double,3,Eigen::Affine> Transform3d;

  class Joint;

  class Link {
  private:
    double length;
    Joint* childJoint;

  public:
    Link(const double length_ = 1, Joint* child_ = NULL);

    void draw();

    void GetEndEffector(Transform3d& t, Eigen::Vector3d& currEndEffector);

  };


}

#endif
