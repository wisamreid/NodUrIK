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
    Joint* childJoint;

  public:
    Link(const double length_ = 1, Joint* child_ = NULL);

    void draw();

  };


}

#endif
