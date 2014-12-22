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

#define HINGE_UPPER M_PI/6
#define HINGE_LOWER -M_PI/6
#define UNIV1_UPPER M_PI/4
#define UNIV1_LOWER -M_PI/4
#define UNIV2_UPPER M_PI/4
#define UNIV2_LOWER -M_PI/4
#define BALL1_UPPER M_PI/2
#define BALL1_LOWER -M_PI/2
#define BALL2_UPPER M_PI/2
#define BALL2_LOWER -M_PI/2
#define BALL3_UPPER M_PI // z-axis rotation
#define BALL3_LOWER -M_PI // x-axis rotation
#define SLIDR_UPPER 1.0
#define SLIDR_LOWER 0.0

#define NUM_SLICES 20
#define NUM_STACKS 10

float joint_amb[3] = {0.0f, 0.0f, 0.7f};
float joint_diff[3] = {0.0f, 0.0f, 0.7f};
float joint_spec[3] = {0.0f, 0.0f, 0.5f};

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
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, joint_amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, joint_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, joint_spec);

  switch (type)
  {
    case HINGE: {
      glRotatef(90,0,1,0);
      glTranslatef(0,0,-size/2);
      // Draw cylinder
      GLUquadricObj *hinge = gluNewQuadric();
      gluCylinder(hinge, size*0.8, size*0.8, size, NUM_SLICES, NUM_STACKS);

      GLfloat x,y;
      // Draw bottom
      // gluDisk(GLUquadric* quad,0,size,NUM_SLICES,NUM_STACKS,1);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0.0f, 0.0f, 0.0f);
      for (GLfloat angle = 0.0f; angle<(2.0f*M_PI); angle+=(M_PI/NUM_SLICES*2)) {
        x = size*0.8*sin(angle);
        y = size*0.8*cos(angle);
        glVertex3f(x, y, 0.0f);
      }
      glEnd();

      // Draw top
      // glTranslatef(0,0,size);
      // gluDisk(GLUquadric* quad,0,size,NUM_SLICES,NUM_STACKS,1);
      // glTranslatef(0,0,-size);
      glBegin(GL_TRIANGLE_FAN);
      glVertex3f(0.0f, 0.0f, size);
      for (GLfloat angle = 0.0f; angle<(2.0f*M_PI); angle+=(M_PI/NUM_SLICES*2)) {
        x = size*0.8*sin(angle);
        y = size*0.8*cos(angle);
        glVertex3f(x, y, size);
      }
      glEnd();

      glTranslatef(0,0,size/2);
      glRotatef(-90,0,1,0);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
    }
      break;
    case UNIVERSAL: {
      glutSolidSphere(size*0.95, NUM_SLICES, NUM_STACKS);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
      glRotatef(thetas[1]/M_PI*180,0,1,0);
    }
      break;
    case BALL: {
      glutSolidSphere(size*0.95, NUM_SLICES, NUM_STACKS);

      // Update for childLink
      glRotatef(thetas[0]/M_PI*180,1,0,0);
      glRotatef(thetas[1]/M_PI*180,0,1,0);
      glRotatef(thetas[2]/M_PI*180,0,0,1);
    }
      break;
    case SLIDER: {

      // currTheta;
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
