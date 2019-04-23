#ifndef _GLOBALLIB_
#define _GLOBALLIB_

#include <GL/glew.h>
#include <GLFW/glfw3.h>

// STRUCT
struct RGB {
	GLfloat R; // Red
	GLfloat G; // Green
	GLfloat B; // Blue
	GLfloat A; // Alpha
};

// APP SETTINGS
#define APP_NAME "SUZUKI JIMNY"
#define APP_WIDTH 1024
#define APP_HEIGHT 1024
#define APP_INPUT_SEPARATOR "====="

#define GRID_SIZE 20

#define FOURCC_DXT1 0x31545844 // Equivalent to "DXT1" in ASCII
#define FOURCC_DXT3 0x33545844 // Equivalent to "DXT3" in ASCII
#define FOURCC_DXT5 0x35545844 // Equivalent to "DXT5" in ASCII

#endif