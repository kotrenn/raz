NAME = hw3
OBJS = main.o common.o vector3d.o ray.o shape.o\
       model.o scene.o camera.o light.o material.o\
       matlib.o transform.o glslprogram.o
FLAGS = -O0 -g -Wall -Wextra -pedantic
#FLAGS = -O3 -Wall -Wextra -pedantic
LINKS = -lm -lopengl32 -lglu32 -lglut32 -lglew32

${NAME}.exe: ${OBJS}
	g++ -o ${NAME} ${OBJS} ${FLAGS} ${LINKS}

.cpp.o: $<
	-g++ ${FLAGS} -c -o $@ $<