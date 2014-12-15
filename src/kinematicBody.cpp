#include "kinematicBody.h"

using namespace stl;

KinematicBody::KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* root_, Link* tip_) : links(links_), joints(joints_), tipLink(tip_), rootLink(root_), numDOFS(0) {
  for (int i=0; i<joints.size(); i++){
    numDOFS+=joints[i]->GetNumDOFS();
  }
}

void KinematicBody::draw() {
  rootLink->draw();
}

void KinematicBody::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  tipLink->GetEndEffector(currEndEffector);
}

void KinematicBody::SolveIK(Eigen::Vector3d& target) {


  // Eigen::Matrix<3,numDOFS> J;
  // /*
  // computeJacobian(J, target);

  //   // m.rows() // num rows
  // // m.cols() // num cols

  // int colIndex = 0;
  // for(int j = 0; j<joints.size(); j++) {
  //   joints[j].computeJacobian(m, colIndex, endEffector);
  // }*/

  // Vector3d delta = target-currentEndEffector();
  // delta/=delta.norm(); // Consider using an l-inf type of normalization

  // Matrix& deltaDofs;
  // deltaDofs = J.jacobiSvd(ComputeThinU | ComputeThinV).solve(delta); // Solve delta = J.dot(deltaDofs)
  // dofs += deltaDofs;
  // // TODO: Figure out if we should check for joint limits here or while solving for the deltaDofs



  // Eigen::Vector3d currEndEffector = Eigen::Vector3d();
  // GetEndEffector(currEndEffector);

}