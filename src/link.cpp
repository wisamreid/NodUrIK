#include "link.h"

using namespace stl;

#define NUM_SLICES 30
#define NUM_STACKS 10

float link_amb[3] = {0.7f, 0.7f, 0.7f};
float link_diff[3] = {0.7f, 0.7f, 0.7f};
float link_spec[3] = {0.5f, 0.5f, 0.5f};

Link::Link(const double length_, const double width_, Joint* child_) : length(length_), width(width_), childJoint(child_) {}

void Link::draw() {
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, link_amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, link_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, link_spec);

  // Draw cone
  glutSolidCone(width, length, NUM_SLICES, NUM_STACKS);

  GLfloat x,y;
  // Draw bottom
  // gluDisk(GLUquadric* quad,0,size,NUM_SLICES,NUM_STACKS,1);
  glBegin(GL_TRIANGLE_FAN);
  glVertex3f(0.0f, 0.0f, 0.0f);
  for (GLfloat angle = 0.0f; angle<(2.0f*M_PI)+(M_PI/NUM_SLICES*2); angle+=(M_PI/NUM_SLICES*2)) {
    x = width*sin(angle);
    y = width*cos(angle);
    glVertex3f(x, y, 0.0f);
  }
  glEnd();

  if (childJoint) {
    glTranslatef(0,0,length);
    childJoint->draw();
  }
}

void Link::GetEndEffector(Eigen::Vector3d& currEndEffector) {
  currEndEffector = (globalTransform * Eigen::Translation3d(0,0,length)).translation();
}

void Link::UpdateTransform(Transform3d& currGlobalTransform) {
  globalTransform = currGlobalTransform;
  if (childJoint) {
    Transform3d tempTransform = globalTransform * Eigen::Translation3d(0,0,length);
    childJoint->UpdateTransform(tempTransform);
  }
}
