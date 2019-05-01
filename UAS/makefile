all: main run

main: main.cpp
	g++ -o main controls.cpp shader.cpp texture.cpp main.cpp -lglfw3 -lopengl32 -lgdi32 -lglew32

run: main
	main