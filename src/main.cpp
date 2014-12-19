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

#define OSCPKT_OSTREAM_OUTPUT
#include "oscpkt.hpp"
#include "udp.hpp"
#define PORT_NUM 7403

#define PI 3.14159265  // Should be used from mathlib
//#define SPACEBAR ' '
#define SPACEBAR 32
#define SHIFTKEY 31

// #define INIT_WINDOW_WIDTH 800.0
// #define INIT_WINDOW_HEIGHT 800.0

#define USE_UDP true

using namespace stl;

//****************************************************
// Global Variables
//****************************************************
Viewport  viewport;
std::vector<KinematicBody*> kinBodies;

oscpkt::UdpSocket sock; 

float target_amb[3] = {0.0f, 0.5f, 0.0f};
float target_diff[3] = {0.0f, 1.0f, 0.0f};
float target_spec[3] = {1.0f, 1.0f, 1.0f};

float path_amb[3] = {1.0f, 0.2f, 0.2f};
float path_diff[3] = {1.0f, 0.2f, 0.2f};
float path_spec[3] = {0.5f, 0.0f, 0.0f};

enum Frame {
  GLOBAL = 0,
  PATH,
  NUM_FRAMES
};

Frame currFrame = GLOBAL;
bool playAnimation = true;

Eigen::Vector3d trans[NUM_FRAMES] = {Eigen::Vector3d(0,0,-20),Eigen::Vector3d(-3,0,6)};
Eigen::Vector3d rot[NUM_FRAMES] = {Eigen::Vector3d(0,0,0),Eigen::Vector3d(0,0,0)};

std::vector<Eigen::Vector3d> pointPath;
int currPathPoint = 0;
Transform3d currPathTransform;

void bezCurveInterp(std::vector<Eigen::Vector3d>& curve, double u, Eigen::Vector3d& result){
  Eigen::Vector3d A, B, C, D, E;

  // split each of the three segments
  // to form two new ones AB and BC
  A = curve[0] * (1.0-u) + curve[1] * u;
  B = curve[1] * (1.0-u) + curve[2] * u;
  C = curve[2] * (1.0-u) + curve[3] * u;

  // split AB and BC to form a new segment DE
  D = A * (1.0-u) + B * u;
  E = B * (1.0-u) + C * u;

  // this is the point on the curve
  result = D * (1.0-u) + E * u;
};

void updatePathTransform() {
  // Apply path transform
  glLoadIdentity(); 
  glTranslatef(trans[PATH][0], trans[PATH][1], trans[PATH][2]);
  glRotatef(rot[PATH][0],1.0,0.0,0.0);
  glRotatef(rot[PATH][1],0.0,1.0,0.0);
  glRotatef(rot[PATH][2],0.0,0.0,1.0);

  GLfloat matrix[16]; 
  glGetFloatv(GL_MODELVIEW_MATRIX, matrix);
  Eigen::Matrix4d temp;
  temp << matrix[0], matrix[4], matrix[8],  matrix[12],
          matrix[1], matrix[5], matrix[9],  matrix[13],
          matrix[2], matrix[6], matrix[10], matrix[14],
          matrix[3], matrix[7], matrix[11], matrix[15];
  currPathTransform = temp;
}

void initPath(){
  std::vector< std::vector<Eigen::Vector3d> > bezPath;
  std::vector<Eigen::Vector3d> bezCurve;

  bezCurve.push_back(Eigen::Vector3d(0,0,0));
  bezCurve.push_back(Eigen::Vector3d(0,4,0));
  bezCurve.push_back(Eigen::Vector3d(6,4,0));
  bezCurve.push_back(Eigen::Vector3d(6,0,0));
  bezPath.push_back(bezCurve);
  bezCurve.clear();

  bezCurve.push_back(Eigen::Vector3d(6,0,0));
  bezCurve.push_back(Eigen::Vector3d(6,-4,0));
  bezCurve.push_back(Eigen::Vector3d(0,-4,0));
  bezCurve.push_back(Eigen::Vector3d(0,0,0));
  bezPath.push_back(bezCurve);
  bezCurve.clear();

  bezCurve.push_back(Eigen::Vector3d(0,0,0));
  bezCurve.push_back(Eigen::Vector3d(0,4,0));
  bezCurve.push_back(Eigen::Vector3d(-6,4,0));
  bezCurve.push_back(Eigen::Vector3d(-6,0,0));
  bezPath.push_back(bezCurve);
  bezCurve.clear();

  bezCurve.push_back(Eigen::Vector3d(-6,0,0));
  bezCurve.push_back(Eigen::Vector3d(-6,-4,0));
  bezCurve.push_back(Eigen::Vector3d(0,-4,0));
  bezCurve.push_back(Eigen::Vector3d(0,0,0));
  bezPath.push_back(bezCurve);
  bezCurve.clear();

  double param = 0.01;
  int num_steps = (int)(1/param)+1;
  Eigen::Vector3d point;

  for (int j=0; j<bezPath.size(); j++){
    for (int u = 0; u*param < 1; u++) {
      bezCurveInterp(bezPath[j],u*param, point);
      pointPath.push_back(point);
    }
  }

  updatePathTransform();

}

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
  std::vector<Link*> links;
  std::vector<Joint*> joints;

  // 1 Link, 0 Joint Arm
  // Link* tip = new Link(1);
  // links.push_back(tip);

  // KinematicBody* linkNoJoint = new KinematicBody(links,joints,tip,tip);
  // kinBodies.push_back(linkNoJoint);

  // 2 Links, 1 Joint Arm
  // Link* tip = new Link();
  // links.push_back(tip);
  // Joint* joint1 = new Joint(tip, UNIVERSAL);
  // joints.push_back(joint1);
  // Link* base = new Link(3, 0.5, joint1);
  // links.push_back(base);

  // KinematicBody* linksAndJoint = new KinematicBody(links,joints,base,tip);
  // kinBodies.push_back(linksAndJoint);

  // 3 Links, 2 Joint Arm
  // Link* tip = new Link(2,0.5);
  // links.push_back(tip);
  // Joint* joint1 = new Joint(tip, HINGE);
  // joints.push_back(joint1);
  // Link* mid = new Link(2,0.5,joint1);
  // links.push_back(mid);
  // Joint* joint2 = new Joint(mid, HINGE);
  // joints.push_back(joint2);
  // Link* base = new Link(3, 0.5, joint2);
  // links.push_back(base);

  // KinematicBody* linksAndJoints = new KinematicBody(links,joints,base,tip);
  // kinBodies.push_back(linksAndJoints);

  // 4 Links, 3 Joint Arm
  Link* tip = new Link(2,0.5);
  links.push_back(tip);
  Joint* joint1 = new Joint(tip, BALL);
  joints.push_back(joint1);
  Link* midTip = new Link(4,0.5,joint1);
  links.push_back(midTip);
  Joint* joint2 = new Joint(midTip, BALL);
  joints.push_back(joint2);
  Link* baseMid = new Link(3, 0.5, joint2);
  links.push_back(baseMid);
  Joint* joint3 = new Joint(baseMid, BALL);
  joints.push_back(joint3);
  Link* base = new Link(2, 0.5, joint3);
  links.push_back(base);

  KinematicBody* linksAndJoints = new KinematicBody(links,joints,base,tip);
  kinBodies.push_back(linksAndJoints);

}

void initUDP() {
  sock.bindTo(PORT_NUM);
  if (!sock.isOk()) {
    std::cerr << "Error opening port " << PORT_NUM << ": " << sock.errorMessage() << "\n";
  } else {
    std::cout << "Server started, will listen to packets on port " << PORT_NUM << std::endl;
  }
}

void initScene(){

  glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
  glClearDepth(1.0f);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_POLYGON_SMOOTH);
  glDepthFunc(GL_LEQUAL);
  glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  initKinBodies();
  initPath();
  initUDP();

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
//****************************************************
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

void checkUDP(Eigen::Vector3d& target) {
  static Eigen::Vector3d currentPos(0,0,0);
  oscpkt::PacketReader pr;
  oscpkt::PacketWriter pw;
  if (sock.isOk()) {
    if (sock.receiveNextPacket(10 /* timeout, in ms */)) {
      pr.init(sock.packetData(), sock.packetSize());
      oscpkt::Message *msg;
      if (pr.isOk() && (msg = pr.popMessage()) != 0) {
        float farg;
        int iarg;
        oscpkt::Message::ArgReader args(*msg);
        
        std::cout << "[";
        args.popInt32(iarg);
        args.popFloat(farg);
        currentPos[0] = farg*4;
        std::cout << farg*4 <<", ";
        args.popFloat(farg);
        currentPos[1] = farg*4;
        std::cout << farg*4 <<", ";
        args.popFloat(farg);
        currentPos[2] = farg*4;
        std::cout << farg*4;
        std::cout << "]" << std::endl;


      } else {
        std::cout << "PacketReader failed!" << std::endl;
      }
    }
  } else {
    std::cout << "Socket not OK!" << std::endl;
  }
  target = currentPos;
}

void drawPath() {
  // Apply path transform
  glTranslatef(trans[PATH][0], trans[PATH][1], trans[PATH][2]);
  glRotatef(rot[PATH][0],1.0,0.0,0.0);
  glRotatef(rot[PATH][1],0.0,1.0,0.0);
  glRotatef(rot[PATH][2],0.0,0.0,1.0);

  double param = 0.01;
  int num_steps = (int)(1/param)+1;
  Eigen::Vector3d point;

  glLineWidth(4);

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, path_amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, path_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, path_spec);

  // Draw whole path
  glBegin(GL_LINE_STRIP);
    for (int i=0; i<pointPath.size(); i++) {
      glVertex3f(pointPath[i][0],pointPath[i][1],pointPath[i][2]);
    }
  glEnd();

  // Undo path transform
  glRotatef(-rot[PATH][2],0.0,0.0,1.0);
  glRotatef(-rot[PATH][1],0.0,1.0,0.0);
  glRotatef(-rot[PATH][0],1.0,0.0,0.0);
  glTranslatef(-trans[PATH][0], -trans[PATH][1], -trans[PATH][2]);
}

void drawKinBodies(Eigen::Vector3d& target) {
  std::vector<KinematicBody*>::iterator KBiter;

  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, target_amb);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, target_diff);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, target_spec);

  // Draw target
  glTranslatef(target[0], target[1], target[2]);
  glutSolidSphere(0.2, 30, 30);
  glTranslatef(-target[0], -target[1], -target[2]);

  // Draw bodies
  for(KBiter=kinBodies.begin(); KBiter != kinBodies.end(); KBiter++) {
    (*KBiter)->SolveIK(target);
    (*KBiter)->draw();
  }

}

void drawFloor() {
  glColor4f(0.7, 0.0, 0.0, 0.40);
  
  glDisable(GL_LIGHTING);
  glBegin(GL_QUADS);
    glVertex3f(-10.0, 12.0, 0.0);
    glVertex3f(12.0, 12.0, 0.0);
    glVertex3f(12.0, -10.0, 0.0);
    glVertex3f(-10.0, -10.0, 0.0);
  glEnd();
  glEnable(GL_LIGHTING);
}

void myDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);                // clear the color buffer (sets everything to black), and the depth buffer.

  glMatrixMode(GL_MODELVIEW);			        // indicate we are specifying camera transformations

  glLoadIdentity();                            // make sure transformation is "zero'd"

  // Apply global transform
  glTranslatef(trans[GLOBAL][0], trans[GLOBAL][1], trans[GLOBAL][2]);
  glRotatef(rot[GLOBAL][0],1.0,0.0,0.0);
  glRotatef(rot[GLOBAL][1],0.0,1.0,0.0);
  glRotatef(rot[GLOBAL][2],0.0,0.0,1.0);

  // Code to draw objects

  Eigen::Vector3d target(0,0,0);

  if (USE_UDP) {
    checkUDP(target);
    target = currPathTransform*target;
  } else {
    target = currPathTransform*pointPath[currPathPoint];
  }



  // Draw reflection
  glPushMatrix();
    glScalef(1, 1, -1);
    //   setLightPositions();
    drawPath();
    drawKinBodies(target);
  glPopMatrix();

  // Draw floor
  // setLightPositions();
  glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    drawFloor();
  glDisable(GL_BLEND);

  // Draw objects
  drawPath();
  drawKinBodies(target);


  // Other glut calls
  // glutPostRedisplay();
  glFlush();
  glutSwapBuffers();					// swap buffers (we earlier set double buffer)
}

//****************************************************
// function to process keyboard input
//****************************************************
void myKeyboard(unsigned char key, int x, int y) {
  if (key == 'q') {
    exit(0);
  }
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
  if (key == 'p') {
    //toggle between play and pause
    playAnimation = !playAnimation;
  }
  if (key == '+') {
    trans[currFrame][2]+=0.1;
  }
  if (key == '-') {
    trans[currFrame][2]-=0.1;
  }
  if (key == '[') {
    rot[currFrame][2]-=2;
  }
  if (key == ']') {
    rot[currFrame][2]+=2;
  }
  if (key == SPACEBAR) {
    currFrame = (Frame)((currFrame+1)%NUM_FRAMES);
  }
  updatePathTransform();
  glutPostRedisplay();
}

void myArrowKeys(int key, int x, int y) {
  int modifier = glutGetModifiers();
  if (modifier&GLUT_ACTIVE_SHIFT != 0) {
    //Translate
    switch(key) {
      case GLUT_KEY_UP:
        trans[currFrame][1]+=0.1;
        break;
      case GLUT_KEY_DOWN:
        trans[currFrame][1]-=0.1;
        break;
      case GLUT_KEY_LEFT:
        trans[currFrame][0]-=0.1;
        break;
      case GLUT_KEY_RIGHT:
        trans[currFrame][0]+=0.1;
        break;
    }
  } else {
    //Rotate
    switch(key) {
      case GLUT_KEY_UP:
        rot[currFrame][0]+=2;
        break;
      case GLUT_KEY_DOWN:
        rot[currFrame][0]-=2;
        break;
      case GLUT_KEY_LEFT:
        rot[currFrame][1]-=2;
        break;
      case GLUT_KEY_RIGHT:
        rot[currFrame][1]+=2;
        break;
    }
  }
  updatePathTransform();
  glutPostRedisplay();
}

//****************************************************
// function to process mouse input
//****************************************************
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
// function to create animation
//****************************************************
void myTimer(int value){
    if (playAnimation) {
      currPathPoint = (currPathPoint+1)%pointPath.size();
      glutPostRedisplay();
    }
    glutTimerFunc(25,myTimer,0); // Call myTimer every 25 milliseconds
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
  glutTimerFunc(25,myTimer,0);            // function to start the animation loop
  glutMainLoop();                         // infinite loop that will keep drawing and resizing

  return 0;
}
