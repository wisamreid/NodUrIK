#ifndef JOINT_H
#define JOINT_H

#include <iostream>
#include <Eigen/Core>
#include "link.h"

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class Link;

  class Joint {
  private:
    Eigen::Vector3d anchor;
    Eigen::Vector3d axis;
    Link* child;

  public:
    Joint() {};
    Joint(const Eigen::Vector3d& anchor_, const Eigen::Vector3d& axis_, const Link child_) {};

    inline void stream_to(std::ostream& os) const {os<< anchor <<", "<< axis << ", " << child;};
  };

  static inline std::ostream& operator << (std::ostream& os, const Joint& j) {j.stream_to(os); return os;}

}

#endif
