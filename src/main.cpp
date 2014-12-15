#include <iostream>
#include <fstream>
#include <cmath>
#include <cstdlib>
#include <cassert>

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

#include <time.h>
#include <math.h>

#include <vector>
#include "joint.h"
#include "kinematicBody.h"
#include "link.h"
#include "viewport.h"


#define PI 3.14159265  // Should be used from mathlib
//#define SPACEBAR ' '
#define SPACEBAR 32
#define SHIFTKEY 31

// #define INIT_WINDOW_WIDTH 800.0
// #define INIT_WINDOW_HEIGHT 800.0

using namespace stl;

//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
std::vector<KinematicBody*> kinBodies;


//****************************************************
// reshape viewport if the window is resized
//****************************************************
void myReshape(int w, int h) {
  viewport.resize(w,h);

  glViewport (0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  if (h == 0) { h=1;}
  float ratio = (float)w /(float)h;
  glLoadIdentity();
  gluPerspective(45.0f, ratio, 0.1f, 200.0f);
}


//****************************************************
// Simple init function
//****************************************************

void initKinBodies() {
  // 1 Link, 0 Joint Arm
  std::vector<Link*> links;
  std::vector<Joint*> joints;

  Link* tip = new Link(1);
  links.push_back(tip);

  KinematicBody* linkNoJoint = new KinematicBody(links,joints,tip,tip);
  kinBodies.push_back(linkNoJoint);

  // 2 Links, 1 Joint Arm
  // vector<*Link> links;
  // vector<*Joint> joints;

  // Link* tip = new Link(1);
  // links.push_back(tip);
  //
  // KinematicBody* linkNoJoint = new KinematicBody(links,joints,tip);
  // kinBodies.push_back(linkNoJoint);

  // 2 Links, 1 Joint Arm
  Link* tip = new Link(1);
  links.push_back(tip);
  Joint* joint1 = new Joint(tip, UNIVERSAL);
  joints.push_back(joint1);
  Link* base = new Link(2,joint1);
  links.push_back(base);

  KinematicBody* linksAndJoint = new KinematicBody(links,joints,base);
  kinBodies.push_back(linksAndJoint);

}

void initScene(){

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  initKinBodies();

//   GLuint depthTexture;
//   glGenTextures(1, &depthTexture);
//   glBindTexture(GL_TEXTURE_2D, depthTexture);

  GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat mat_shininess[] = { 50.0 };
  GLfloat light_position[] = { 1.0, 1.0, 1.0, 0.0 };
  glClearColor (0.0, 0.0, 0.0, 0.0);
  
  glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
  glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  
  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

}

//****************************************************
// functions that do the actual drawing of stuff
//***************************************************
// void setColor(int index) {
//   if (index==current_obj) {
//     if (mode==FILLED) {
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, curr_fill_amb);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, curr_fill_diff);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, curr_fill_spec);
//     } else {
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, curr_wire_amb);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, curr_wire_diff);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, curr_wire_spec);
//     }
//   } else {
//     if (mode==FILLED) {
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, fill_amb);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, fill_diff);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, fill_spec);
//     } else {
//       glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, wire_amb);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, wire_diff);
//       glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, wire_spec);
//     }
//   }
// }
//
// void drawObjects() {
//   int obj_index;
//   for (int k=0; k<bezObjs.size(); k++){
//     obj_index = bezObjs[k][0].getIndex();
//     setColor(obj_index);
//     glLoadIdentity(); // make sure transformation is "zero'd"
//     glTranslatef(trans[obj_index][0], trans[obj_index][1], trans[obj_index][2]);
//     glRotatef(rot[obj_index][0],1.0,0.0,0.0);
//     glRotatef(rot[obj_index][1],0.0,1.0,0.0);
//     glRotatef(rot[obj_index][2],0.0,0.0,1.0);
//     glTranslatef(-obj_centers[obj_index][0], -obj_centers[obj_index][1], -obj_centers[obj_index][2]);
//     for (int i=0; i<bezObjs[k].size(); i++) {
//       bezObjs[k][i].draw();
//     }
//   }
//   for (int k=0; k<objModels.size(); k++) {
//     obj_index = objModels[k].getIndex();
//     setColor(obj_index);
//     glLoadIdentity(); // make sure transformation is "zero'd"
//     glTranslatef(trans[obj_index][0], trans[obj_index][1], trans[obj_index][2]);
//     glRotatef(rot[obj_index][0],1.0,0.0,0.0);
//     glRotatef(rot[obj_index][1],0.0,1.0,0.0);
//     glRotatef(rot[obj_index][2],0.0,0.0,1.0);
//     glTranslatef(-obj_centers[obj_index][0], -obj_centers[obj_index][1], -obj_centers[obj_index][2]);
//     objModels[k].draw();
//   }
// }

void myDisplay() {

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations

  glLoadIdentity();                            // make sure transformation is "zero'd"

  glTranslatef(0, 0, -15);

  // // Code to draw objects

  std::vector<KinematicBody*>::iterator KBiter;

  for(KBiter=kinBodies.begin(); KBiter != kinBodies.end(); KBiter++) {
    (*KBiter)->draw();
  }

  // if (shading==FLAT) glShadeModel(GL_FLAT);
  // else glShadeModel(GL_SMOOTH);
  //
  // if (mode==FILLED) {
  //   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  // }
  // else {
  //   glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  // }
  // drawObjects();
  //
  // if (mode==HIDDEN) {
  //   // http://www.glprogramming.com/red/chapter14.html#name16
  //   glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  //   glEnable(GL_POLYGON_OFFSET_FILL);
  //   glPolygonOffset(1.0, 1.0);
  //   glColor3f(0.0f, 0.0f, 0.0f); // Background color
  //   glDisable(GL_LIGHTING);
  //   drawObjects();
  //   glEnable(GL_LIGHTING);
  //   glDisable(GL_POLYGON_OFFSET_FILL);
  // }
  //
  glutPostRedisplay();
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// function to process keyboard input
//***************************************************
void myKeyboard(unsigned char key, int x, int y) {
  // if (key == 'q') {
  //   exit(0);
  // }
  // if (key == 's') {
  //   //toggle between flat and smooth shading
  //   if (shading==SMOOTH) shading=FLAT;
  //   else shading=SMOOTH;
  // }
  // if (key == 'w') {
  //   //toggle between filled and wireframe mode
  //   if (mode!=WIREFRAME) mode=WIREFRAME;
  //   else mode=FILLED;
  // }
  // if (key == 'h') {
  //   //toggle between filled and hidden-line mode
  //   if (mode!=HIDDEN) mode=HIDDEN;
  //   else mode=WIREFRAME;
  // }
  // if (key == '+') {
  //   trans[current_obj][2]+=0.1;
  // }
  // if (key == '-') {
  //   trans[current_obj][2]-=0.1;
  // }
  // if (key == SPACEBAR) {
  //   current_obj = (current_obj+1)%(bezObjs.size()+objModels.size());
  // }
  //
  // glutPostRedisplay();
}

void myArrowKeys(int key, int x, int y) {
  // int modifier = glutGetModifiers();
  // if (modifier&GLUT_ACTIVE_SHIFT != 0) {
  //   //Translate
  //   switch(key) {
  //     case GLUT_KEY_UP:
  //       trans[current_obj][1]+=0.1;
  //       break;
  //     case GLUT_KEY_DOWN:
  //       trans[current_obj][1]-=0.1;
  //       break;
  //     case GLUT_KEY_LEFT:
  //       trans[current_obj][0]-=0.1;
  //       break;
  //     case GLUT_KEY_RIGHT:
  //       trans[current_obj][0]+=0.1;
  //       break;
  //   }
  // } else {
  //   //Rotate
  //   switch(key) {
  //     case GLUT_KEY_UP:
  //       rot[current_obj][0]+=2;
  //       break;
  //     case GLUT_KEY_DOWN:
  //       rot[current_obj][0]-=2;
  //       break;
  //     case GLUT_KEY_LEFT:
  //       rot[current_obj][1]-=2;
  //       break;
  //     case GLUT_KEY_RIGHT:
  //       rot[current_obj][1]+=2;
  //       break;
  //   }
  // }
  //
  // glutPostRedisplay();
}

//****************************************************
// function to process mouse input
//***************************************************
void myMouse(int button, int state, int x, int y) {
  //Do something later.
  //Possible button inputs: GLUT_LEFT_BUTTON, GLUT_RIGHT_BUTTON, or GLUT_MIDDLE_BUTTON
  //Possible state inputs: GLUT_UP or GLUT_DOWN
  // glutPostRedisplay();
}

void myMouseMotion(int mouseX, int mouseY) {
  // // Location of the center of pixel relative to center of sphere
  // double radius = mouseLightRadius;
  // double x = (mouseX-viewport.getW()/2.0);
  // double y = -(mouseY-viewport.getH()/2.0);
  // double dist = sqrt(std::pow(x,2) + std::pow(y,2));
  // if (dist > radius) { dist = radius;}
  // double z = sqrt(radius*radius-dist*dist);
  // if (lights.size()>0){
  //   lights[mouseLight]->moveLight(Vector(x,y,z));
  // }

  // glutPostRedisplay();
}

//****************************************************
// the usual stuff, nothing exciting here
//****************************************************
int main(int argc, char *argv[]) {

  //This parses the arguments and sets up the global variables
  // Parser argParser = Parser();
  // argParser.parse(&argc, argv);

  //This initializes glut
  glutInit(&argc, argv);

  //This tells glut to use a double-buffered window with red, green, and blue channels, and a depth buffer.
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

  // Initalize theviewport
  viewport = Viewport(400,400);

  //The size and position of the window
  glutInitWindowSize(viewport.getW(), viewport.getH());
  glutInitWindowPosition(0,0);
  glutCreateWindow(argv[0]);

  initScene();							// quick function to set up scene

  glutKeyboardFunc(myKeyboard);           // function to run when its time to read keyboard input
  glutSpecialFunc(myArrowKeys);           // function to run when arrow keys are pressed
  glutMotionFunc(myMouseMotion);          // function to run when mouse input is received
  glutMouseFunc(myMouse);                 // function to run when mouse input is received
  glutDisplayFunc(myDisplay);             // function to run when its time to draw something
  glutReshapeFunc(myReshape);             // function to run when the window gets resized
  glutMainLoop();                         // infinite loop that will keep drawing and resizing

  return 0;
}
