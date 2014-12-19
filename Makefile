CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
  CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I/usr/X11/include -DOSX -I ./eigen/
  LDFLAGS = -framework GLUT -framework OpenGL \
      -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
      -lGL -lGLU -lm -lstdc++
else
  CFLAGS = -g -std=c++0x -DGL_GLEXT_PROTOTYPES -I./include/ -Iglut-3.7.6-bin -I ./eigen/ 
  LDFLAGS = -lglut -lGLU
endif

OUTPUT = final
SOURCES = src/main.cpp src/viewport.cpp src/joint.cpp src/kinematicBody.cpp src/link.cpp
#SOURCES = $(shell find . -name *.cpp)
#SOURCES=$(wildcard src/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

RM = /bin/rm -f
all: $(OUTPUT)
$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) $(OUTPUT) src/*.o
