all: main run

main: main.cpp
	g++ -o main main.cpp -I../include -L../lib -lglfw3 -lopengl32 -lgdi32

run: main
	main