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

#endif