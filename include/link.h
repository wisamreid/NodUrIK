#ifndef LINK_H
#define LINK_H

#include <Eigen/Core>
#include "joint.h"

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class Joint;

  class Link {
  private:
    double length;
    Joint* joint;

  public:
    Link();
    Link(const double length_, const Joint& joint_);

  };


}

#endif
