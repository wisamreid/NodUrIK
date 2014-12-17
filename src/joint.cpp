#include "joint.h"

#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#endif

#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif

#include <math.h>

using namespace stl;

#define HINGE_UPPER M_PI/2
#define HINGE_LOWER -M_PI/2
#define UNIV1_UPPER M_PI/2
#define UNIV1_LOWER -M_PI/2
#define UNIV2_UPPER M_PI/2
#define UNIV2_LOWER -M_PI/2
#define BALL1_UPPER M_PI/2
#define BALL1_LOWER -M_PI/2
#define BALL2_UPPER M_PI/2
#define BALL2_LOWER -M_PI/2
#define BALL3_UPPER M_PI // z-axis rotation
#define BALL3_LOWER -M_PI // x-axis rotation
#define SLIDR_UPPER 1.0
#define SLIDR_LOWER 0.0

Joint::Joint(Link* child_, JointType type_, double theta1, double theta2, double theta3, double size_) : childLink(child_), type(type_), size(size_)  {
  switch (type)
  {
    case HINGE:
      thetas.push_back(theta1);
      break;
    case UNIVERSAL:
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      break;
    case BALL:
      thetas.push_back(theta1);
      thetas.push_back(theta2);
      thetas.push_back(theta3);
      break;
    case SLIDER:
      thetas.push_back(theta1);
      break;
  }
}

void Joint::draw() {
  switch (type)
  {
    case HINGE: {
      glRotatef(90,0,1,0);
      glTranslatef(0,0,-size/2);
      GLUquadricObj *hinge = gluNewQuadric();
      gluCylinder(hinge, size, size, size, 30, 30);
      glTranslatef(0,0,size/2);
      glRotatef(-90,0,1,0);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
    }
      break;
    case UNIVERSAL: {
      glutSolidSphere(size, 30, 30);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
      glRotatef(thetas[1]/M_PI*180,0,1,0);
    }
      break;
    case BALL: {
      glutSolidSphere(size, 30, 30);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
      glRotatef(thetas[1]/M_PI*180,0,1,0);
      glRotatef(thetas[2]/M_PI*180,0,0,1);
    }
      break;
    case SLIDER: {
      // Draw Slider Here (Cylinder)
    }
      break;
  }

  if (childLink) {
    childLink->draw();
  }

}

void Joint::UpdateTransform(Transform3d& currGlobalTransform) {
  globalTransform = currGlobalTransform;
  if (childLink) {
    Transform3d tempTransform;
    switch (type)
    {
      case HINGE:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX());
        break;
      case UNIVERSAL:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(thetas[1], Eigen::Vector3d::UnitY());
        break;
      case BALL:
        tempTransform = globalTransform * Eigen::AngleAxisd(thetas[0], Eigen::Vector3d::UnitX()) * Eigen::AngleAxisd(thetas[1], Eigen::Vector3d::UnitY()) * Eigen::AngleAxisd(thetas[2], Eigen::Vector3d::UnitZ());
        break;
      case SLIDER:
        // tempTransform = globalTransform * Eigen::Translation3d(Eigen::Vector3d::UnitZ()*thetas[0]);
        break;
    }
    childLink->UpdateTransform(tempTransform);
  }
}

int Joint::GetNumDOFS() {
  switch (type)
  {
    case HINGE:
      return 1;
      break;
    case UNIVERSAL:
      return 2;
      break;
    case BALL:
      return 3;
      break;
    case SLIDER:
      return 1;
      break;
  }
}

void Joint::GetDOFS(Dofs& dofs, int& currIndex) {
  switch (type)
  {
    case HINGE:
      dofs[currIndex++] = thetas[0];
      break;
    case UNIVERSAL:
      dofs[currIndex++] = thetas[0];
      dofs[currIndex++] = thetas[1];
      break;
    case BALL:
      dofs[currIndex++] = thetas[0];
      dofs[currIndex++] = thetas[1];
      dofs[currIndex++] = thetas[2];
      break;
    case SLIDER:
      dofs[currIndex++] = thetas[0];
      break;
  }
}

void Joint::SetDOFS(Dofs& dofs, int& currIndex) {
  double currTheta;
  switch (type)
  {
    case HINGE:
      currTheta = dofs[currIndex++];
      if (currTheta > HINGE_UPPER) {
        currTheta = HINGE_UPPER;
      } else if (currTheta < HINGE_LOWER) {
        currTheta = HINGE_LOWER;
      }
      thetas[0] = currTheta;
      break;
    case UNIVERSAL:
      currTheta = dofs[currIndex++];
      if (currTheta > UNIV1_UPPER) {
        currTheta = UNIV1_UPPER;
      } else if (currTheta < UNIV1_LOWER) {
        currTheta = UNIV1_LOWER;
      }
      thetas[0] = currTheta;

      currTheta = dofs[currIndex++];
      if (currTheta > UNIV2_UPPER) {
        currTheta = UNIV2_UPPER;
      } else if (currTheta < UNIV2_LOWER) {
        currTheta = UNIV2_LOWER;
      }
      thetas[1] = currTheta;
      break;
    case BALL:
      currTheta = dofs[currIndex++];
      if (currTheta > BALL1_UPPER) {
        currTheta = BALL1_UPPER;
      } else if (currTheta < BALL1_LOWER) {
        currTheta = BALL1_LOWER;
      }
      thetas[0] = currTheta;

      currTheta = dofs[currIndex++];
      if (currTheta > BALL2_UPPER) {
        currTheta = BALL2_UPPER;
      } else if (currTheta < BALL2_LOWER) {
        currTheta = BALL2_LOWER;
      }
      thetas[1] = currTheta;

      currTheta = dofs[currIndex++];
      if (currTheta > BALL3_UPPER) {
        currTheta = BALL3_UPPER;
      } else if (currTheta < BALL3_LOWER) {
        currTheta = BALL3_LOWER;
      }
      thetas[2] = currTheta;
      break;
    case SLIDER:
      currTheta = dofs[currIndex++];
      if (currTheta > SLIDR_UPPER) {
        currTheta = SLIDR_UPPER;
      } else if (currTheta < SLIDR_LOWER) {
        currTheta = SLIDR_LOWER;
      }
      thetas[0] = currTheta;
      break;
  }
}

void Joint::computeJacobian(Jacobian& J, int& currCol, Eigen::Vector3d& endEffector) {
  Eigen::Vector3d partialJac;
  Eigen::Vector3d dir = endEffector-(globalTransform*anchor);
  switch (type)
  {
    case SLIDER:
      // Add Slider Jacobian here!
      break;
    default:
      for(int i=0; i<GetNumDOFS(); i++) {
        Eigen::Vector3d axis;
        switch (i)
        {
          case 0:
            axis = globalTransform.linear()*Eigen::Vector3d::UnitX();
            break;
          case 1:
            axis = globalTransform.linear()*Eigen::Vector3d::UnitY();
            break;
          case 2:
            axis = globalTransform.linear()*Eigen::Vector3d::UnitZ();
            break;
        }
        partialJac = axis.cross(dir);
        J(0,currCol) = partialJac[0];
        J(1,currCol) = partialJac[1];
        J(2,currCol) = partialJac[2];
        currCol++;
      }
  }
}
