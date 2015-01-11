NodUrIK
=======
##########################################
CS184 - Foundations of Computer Graphics
Assignment 3: Bezier Subdivision
##########################################

Projects URL: http://www-inst.cs.berkeley.edu/~cs184-ds/Projects/projects.html
[[/videos/video1.mp4]]

Authors:
----------
Wisam Reid (cs184-ds)
Eddie Groshev (cs184-en)

Platform:
-----------
Mac OSX

Tested on Mac OSX 10.9.5 and Mac OSX 10.6.8 on personal computer(s).

BUILD:
--------
Run 'make' to build

SOURCE FILES:
---------------
src:
---
main.cpp
joint.cpp
link.cpp
kinematicBody.cpp
vertex.cpp
viewpoint.cpp


include:
-------
joint.h
link.h
kinematicBody.h
viewpoint.h

Libraries and credits:
------------------------
eigen
https://github.com/cryos/eigen

oscpkt
http://gruntthepeon.free.fr/oscpkt/

FUNCTIONALITY (SPEC):
-----------------------

1) Satisfies minimum requirements from spec

    see video1.mp4
    
    •	Solving an IK problem on an arm that consists of at least four links that are connected in a serial chain by ball joints. 

    •	The lengths of the segments must not all be the same: the longest segment must be at least twice the length of the shortest. 

    •	The root link should be attached to an immobile base by a ball joint. 

    •	The goal is to position the end of the arm at a specified goal point in 3D. 

    •	The solver should be robust to out-of-reach goals. 

    •	You may use the algorithm described in class or any other algorithm that produces good results. 

Controls:
--------

1) Arrow keys (Move Camera)

2) SpaceBar + Arrow Keys (Rotates Path)


Command line arguments:
----------------------

N/A


FUNCTIONALITY (E/C):
----------------------

1) joint limits 

    see video2.mp4


2) other joints besides ball joints 

    see video2.mp4

3) Nod Ring control of IK arm over UDP

    see https://hellonod.com/
    see video3.mp4



VIDEOS:
---------

video1.mp4
video2.mp4
video3.mp4
