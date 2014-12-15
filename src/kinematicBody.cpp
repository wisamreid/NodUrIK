#include "kinematicBody.h"

using namespace stl;

KinematicBody::KinematicBody() {}
KinematicBody::KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* root_) : links(links_), joints(joints_), rootLink(root_) {}

void KinematicBody::draw() {
  rootLink->draw();
}

void KinematicBody::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  Transform3d t = Transform3d::Identity();
  rootLink->GetEndEffector(t,currEndEffector);
}

void KinematicBody::SolveIK() {
  Eigen::Vector3d currEndEffector = Eigen::Vector3d();
  GetEndEffector(currEndEffector);
}