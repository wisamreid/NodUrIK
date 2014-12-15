#include "link.h"

using namespace stl;

Link::Link(const double length_, Joint* child_) : length(length_), childJoint(child_) {}

void Link::draw() {
  //Even if a parent joint doesn't exist, assume that some parent joint updated the OpenGL matrix stack. Under that assumption, draw this link.

  //Push the following translation to the OpenGL stack: (0,0,length)
  //Why am i using (0,0,length)? Because i'm assuming all links point in the +z direction.
  if (childJoint) childJoint->draw();

      // glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
      // glMatrixMode(GL_MODELVIEW);
      // glLoadIdentity();

      // glRotatef(0.0f, 0.0f, 1.0f, 0.0f);

      // glBegin(GL_TRIANGLES);
      //
      // glColor3f(1.0f, 0.0f, 0.0f); //FRONT
      // glVertex3f(0.0f, 1.0f, 0.0f);
      // glVertex3f(1.0f, -1.0f, 1.0f);
      // glVertex3f(-1.0f, -1.0f, 1.0f);
      //
      // glColor3f(0.0f, 1.0f, 0.0f); //RIGHT
      // glVertex3f(0.0f, 1.0f, 0.0f);
      // glVertex3f(0.0f, -1.0f, -1.0f);
      // glVertex3f(1.0f, -1.0f, 1.0f);
      //
      // glColor3f(0.0f, 0.0f, 1.0f); //LEFT
      // glVertex3f(0.0f, 1.0f, 0.0f);
      // glVertex3f(-1.0f, -1.0f, 1.0f);
      // glVertex3f(0.0f, -1.0f, -1.0f);
      //
      // glEnd();

      //TETAEDRON BY HAND
      glBegin(GL_TRIANGLES);

      //front triangle
      glColor4f(1.0f, 0.0f, 0.0f, 1.0f);
      glVertex3f(0.0f, 5.0f, 0.0f);
      glVertex3f( -5.0f, -5.0f, 0.0f);
      glVertex3f( 5.0f,  -5.0f, 0.0f);

      //right side triangle
      glColor4f(0.0f, 0.0f, 1.0f, 1.0f);
      glVertex3f( 5.0f,  -5.0f, 0.0f);
      glVertex3f(0.0f, 5.0f, 0.0f);
      glVertex3f( 0.0f,  -5.0f, -5.0f);

      //left side triangle
      glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
      glVertex3f( -5.0f, -5.0f, 0.0f);
      glVertex3f(0.0f, 5.0f, 0.0f);
      glVertex3f( 0.0f,  -5.0f, -5.0f);

      //bottom triangle
      glColor4f(0.0f, 1.0f, 0.0f, 1.0f);
      glVertex3f( -5.0f, -5.0f, 0.0f);
      glVertex3f( 5.0f,  -5.0f, 0.0f);
      glVertex3f( 0.0f,  -5.0f, -5.0f);

      glEnd();

}

void Link::GetEndEffector(Transform3d& t, Eigen::Vector3d& currEndEffector) {
  t = t * Eigen::Translation3d(0,0,length);
  if (childJoint) childJoint->GetEndEffector(t,currEndEffector);
  else currEndEffector = t.translation();
}