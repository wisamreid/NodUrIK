#ifndef LINK_H
#define LINK_H

#include <iostream>
#include <Eigen/Core>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class Link {
  private:
    double length;
    Joint joint;

  public:
    Joint() {};
    Joint(const double length_, const Joint& joint_) {};

    inline void stream_to(std::ostream& os) const {os<< length <<", "<< joint;};
  };

  static inline std::ostream& operator << (std::ostream& os, const Link& l) {l.stream_to(os); return os;}

}

#endif
