#include "kinematicBody.h"

using namespace stl;

KinematicBody::KinematicBody() {}
KinematicBody::KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* root_) : links(links_), joints(joints_), rootLink(root_) {}

void KinematicBody::draw() {
  rootLink->draw();
}