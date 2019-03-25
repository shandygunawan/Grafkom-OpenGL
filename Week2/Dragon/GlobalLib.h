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
#define APP_NAME "Dragon"
#define APP_WIDTH 1280
#define APP_HEIGHT 960
#define APP_INPUT_TEXT "dragon.txt"
#define APP_INPUT_SEPARATOR "====="

#endif