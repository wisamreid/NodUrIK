#ifndef KINEMATICBODY_H
#define KINEMATICBODY_H

#include <vector>
#include "link.h"
#include "joint.h"

// stl :: string theory labs (taking over a universe near you)
namespace stl
{

  class KinematicBody {

  private:
    std::vector<Link> links;
    std::vector<Joint> joints;
    Link root;

  public:
    KinematicBody();
    KinematicBody(const std::vector<Link>& links_, const std::vector<Joint>& joints_, const Link& root_);

  };

}

#endif
