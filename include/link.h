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

  typedef Eigen::Matrix<double,Eigen::Dynamic,1> Dofs;
  typedef Eigen::Matrix<double,Eigen::Dynamic,Eigen::Dynamic> Jacobian;
  typedef Eigen::Transform<double,3,Eigen::Affine> Transform3d;

  class Joint;

  class Link {
  private:
    double length;
    double width;
    Joint* childJoint;
    Transform3d globalTransform;

  public:
    Link(const double length_ = 4.0, const double width_ = 0.5, Joint* child_ = NULL);

    void draw();
    void UpdateTransform(Transform3d& currGlobalTransform);
    void GetEndEffector(Eigen::Vector3d& currEndEffector);

  };


}

#endif
