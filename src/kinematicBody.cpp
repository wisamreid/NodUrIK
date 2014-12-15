#include "kinematicBody.h"

using namespace stl;

KinematicBody::KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* tip_, Link* root_) : links(links_), joints(joints_), tipLink(tip_), rootLink(root_) {}

void KinematicBody::draw() {
  rootLink->draw();
}

void KinematicBody::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  tipLink->GetEndEffector(currEndEffector);
}

void KinematicBody::SolveIK() {
  Eigen::Vector3d currEndEffector = Eigen::Vector3d();
  GetEndEffector(currEndEffector);

}