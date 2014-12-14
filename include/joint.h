#ifndef JOINT_H
#define JOINT_H

#include <iostream>
#include <vector>
#include <Eigen/Core>
#include "link.h"

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
