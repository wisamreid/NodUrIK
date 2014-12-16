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
    std::vector<Link*> links;
    std::vector<Joint*> joints;
    Link* rootLink;
    Link* tipLink;
    int numDOFS;

  public:
    KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* root_, Link* tip_);

    void draw();
    void GetEndEffector(Eigen::Vector3d& currEndEffector);
    void SolveIK(Eigen::Vector3d& target);
    void GetDOFS(Dofs& dofs);
    void SetDOFS(Dofs& dofs);

  };

}

#endif
