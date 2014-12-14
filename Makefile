CC = g++
ifeq ($(shell sw_vers 2>/dev/null | grep Mac | awk '{ print $$2}'),Mac)
  CFLAGS = -g -DGL_GLEXT_PROTOTYPES -I./include/ -I ./eigen/ -I/usr/X11/include -DOSX
  LDFLAGS = -framework GLUT -framework OpenGL \
      -L"/System/Library/Frameworks/OpenGL.framework/Libraries" \
      -lGL -lGLU -lm -lstdc++
else
  CFLAGS = -g -std=c++0x -DGL_GLEXT_PROTOTYPES -I./include/ -Iglut-3.7.6-bin
  LDFLAGS = -lglut -lGLU
endif

##CFLAGS = -g -Wall -I ./lodepng/
#-o2
#-Iutil_classes -Icore -Iinterface -Ipng++ -Ialgebra3 -fopenmp
##LDFLAGS =
#-g -Wall

OUTPUT = final
SOURCES = src/main.cpp src/viewport.cpp src/joint.cpp src/kinematicBody.cpp src/link.cpp
#lodepng/lodepng.cpp
#SOURCES = $(shell find . -name *.cpp)
#SOURCES=$(wildcard src/*.cpp)
OBJECTS = $(SOURCES:%.cpp=%.o)

RM = /bin/rm -f
all: $(OUTPUT)
# all: build $(OUTPUT)
# build:
# 	@mkdir -p bin
# 	@mkdir -p bin/src
# 	@mkdir -p bin/lodepng
$(OUTPUT): $(OBJECTS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(OUTPUT) $(LDFLAGS)
%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	$(RM) $(OUTPUT) out.png src/*.o lodepng/*.o
