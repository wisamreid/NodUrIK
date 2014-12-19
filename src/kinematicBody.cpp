#include "kinematicBody.h"

using namespace stl;

#define MAX_ITERATIONS 15
#define EPSILON 1e-3
#define LAMBDA 200

KinematicBody::KinematicBody(const std::vector<Link*>& links_, const std::vector<Joint*>& joints_, Link* root_, Link* tip_) : links(links_), joints(joints_), tipLink(tip_), rootLink(root_), numDOFS(0) {
  for (int i=0; i<joints.size(); i++){
    numDOFS+=joints[i]->GetNumDOFS();
  }

  Transform3d t = Transform3d::Identity(); // TODO: Replace this with KinematicBody transform
  rootLink->UpdateTransform(t);
}

void KinematicBody::draw() {
  rootLink->draw();
}

void KinematicBody::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  tipLink->GetEndEffector(currEndEffector);
}

void KinematicBody::SolveIK(Eigen::Vector3d& target) {
  Jacobian J(3,numDOFS);
  Eigen::Vector3d endEffector;
  Eigen::Vector3d targetDir;
  Dofs deltaDofs = Dofs::Zero(numDOFS);
  Dofs currDofs(numDOFS);

  size_t count = 0;
  do {
    GetEndEffector(endEffector);

    targetDir = target-endEffector;
    // if (targetDir.norm() < EPSILON) break;
    //targetDir/=targetDir.norm(); // TODO: Consider using an l-inf type of normalization


    int colIndex = 0;
    for(int j = 0; j<joints.size(); j++) {
      joints[j]->computeJacobian(J, colIndex, endEffector);
    }

    // Replace NaN with 0.0
    // for (int j=0; j<3; j++) {
    //   for (int i=0; i<numDOFS; i++) {
    //     if (J(j,i) != J(j,i)) {
    //       std::cout<<"GOT NAN IN JACOBIAN"<<std::endl;
    //       std::cout<<J<<std::endl<<std::endl;
    //       J(j,i) = 0.0;
    //     }
    //   }
    // }

    // deltaDofs = (J.transpose()*(J*J.transpose()+LAMBDA*Eigen::MatrixXd::Identity(J.rows(),J.rows())).inverse())*targetDir;
    deltaDofs = (J.transpose()*J + LAMBDA*Eigen::MatrixXd::Identity(J.cols(),J.cols())).inverse()*(J.transpose()*targetDir + LAMBDA*deltaDofs);


    // for (int i=0; i<numDOFS; i++) {
    //   if (deltaDofs[i] != deltaDofs[i]) {
    //     std::cout<<"GOT NAN IN DELTADOFS"<<std::endl;
    //     std::cout<<deltaDofs<<std::endl<<std::endl;
    //     deltaDofs[i] = 0.0;
    //   }
    // }

    // std::cout<<"END:"<<endEffector<<std::endl<<std::endl;
    // std::cout<<"NORM:"<<deltaDofs.norm()<<std::endl;
    // std::cout<<"DIR:"<<targetDir<<std::endl;

    if (deltaDofs.norm() < EPSILON) break;

    // TODO: Figure out if we should check for joint limits here or while solving for the deltaDofs

    GetDOFS(currDofs);
    currDofs+=deltaDofs;
    SetDOFS(currDofs);
  } while (count++ < MAX_ITERATIONS);

}

void KinematicBody::GetDOFS(Dofs& dofs) {
  int currIndex = 0;
  for(int j = 0; j<joints.size(); j++) {
    joints[j]->GetDOFS(dofs, currIndex);
  }
}

void KinematicBody::SetDOFS(Dofs& dofs) {
  int currIndex = 0;
  for(int j = 0; j<joints.size(); j++) {
    joints[j]->SetDOFS(dofs, currIndex);
  }
  Transform3d t = Transform3d::Identity(); // TODO: Replace this with KinematicBody transform
  rootLink->UpdateTransform(t);
}
