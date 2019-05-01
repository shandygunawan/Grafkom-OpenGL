all: main run

main: main.cpp
	g++ -o main main.cpp shader.cpp controls.cpp texture.cpp -IC:\mingw-w64\x86_64-8.1.0-posix-seh-rt_v6-rev0\mingw64\include -L../lib -lglfw3 -lglew32 -lopengl32 -lgdi32

run: main
	main