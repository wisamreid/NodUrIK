#ifndef KINEMATICBODY_H
#define KINEMATICBODY_H

#include <iostream>
#include <vector>

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class KinematicBody {

  private:
    std::vector<links> links;
    std::vector<joint> joints;
    Link root;

  public:
    KinematicBody() {};
    KinematicBody(const vector<link>& links_, const vector<joint>& joints_, const Link& root_) {};

    inline void stream_to(std::ostream& os) const {os<< links <<", "<< joints << ", " << root;};
  };

  static inline std::ostream& operator << (std::ostream& os, const KinematicBody& k) {k.stream_to(os); return os;}

}

#endif
