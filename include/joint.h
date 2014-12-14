#ifndef JOINT_H
#define JOINT_H

#include <iostream>
#include <Eigen/Core>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class Joint {
  private:
    Vector anchor;
    Vector axis;
    Link child;
  public:
    Joint() {};
    Joint(const Vector& anchor_, const Vector& axis_, const Link child_) {};

    inline void stream_to(std::ostream& os) const {os<< anchor <<", "<< axis << ", " << child;};
  };

  static inline std::ostream& operator << (std::ostream& os, const Joint& j) {j.stream_to(os); return os;}

}

#endif
