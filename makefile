NAME = raz
src := $(wildcard *.cpp)
obj := $(src:%.cpp=%.o)
FLAGS = -O0 -g -Wall -Wextra -pedantic -std=c++11
#FLAGS = -O3 -Wall -Wextra -pedantic
#LINKS = -lm -lopengl32 -lglu32 -lglut32 -lglew32
LINKS = -lm -lSDL -lSDL_image -lGL -lGLU -lGLEW

all: ${NAME}

${NAME}: $(obj)
	g++ -o ${NAME} $(obj) ${FLAGS} ${LINKS}

$(obj): %.o: %.cpp
	g++ ${FLAGS} -c -o $@ $<

clean:
	rm -rf *.o
