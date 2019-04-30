#ifndef _GLOBALLIB_
#define _GLOBALLIB_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtc/type_ptr.hpp>

// STRUCT
struct RGB {
	GLfloat R; // Red
	GLfloat G; // Green
	GLfloat B; // Blue
	GLfloat A; // Alpha
};

struct Particle {
	glm::vec3 pos, speed;
	unsigned char r,g,b,a; // Color
	float size, angle, weight;
	float life; // Remaining life of the particle. if < 0 : dead and unused.
	float cameradistance; // *Squared* distance to the camera. if dead : -1.0f

	bool operator<(const Particle& that) const {
		// Sort in reverse order : far particles drawn first.
		return this->cameradistance > that.cameradistance;
	}
};

// APP SETTINGS
#define APP_NAME "SUZUKI JIMNY"
#define APP_WIDTH 1024
#define APP_HEIGHT 1024
#define APP_INPUT_SEPARATOR "====="

// Vertex Location
#define LOCATION_MODEL 0
#define LOCATION_TEXTURE 1
#define LOCATION_PARTICLE_VERTICES 2
#define LOCATION_PARTICLE_PROPERTIES 3
#define LOCATION_PARTICLE_COLOR 4

// PARTICLE
#define PARTICLE_MAX_NUMBER 500

// Other
#define GRID_SIZE 20


// Buffer Data
GLfloat model_vbo_data[] {
		0.8f, -0.4f, 1.0f, 0.8f, -0.4f, 0.5f, 0.8f, -0.7f, 0.9f, 0.8f, -0.4f, 0.5f, 0.8f, -0.7f, 0.9f, 0.8f, -0.7f, 0.6f, // Wheel 1
	    -0.8f, -0.4f, 1.0f, -0.8f, -0.4f, 0.5f, -0.8f, -0.7f, 0.9f, -0.8f, -0.4f, 0.5f, -0.8f, -0.7f, 0.9f, -0.8f, -0.7f, 0.6f, // Wheel 2
	    0.8f, -0.4f, -1.0f, 0.8f, -0.4f, -0.5f, 0.8f, -0.7f, -0.9f, 0.8f, -0.4f, -0.5f, 0.8f, -0.7f, -0.9f, 0.8f, -0.7f, -0.6f, // Wheel 3
	    -0.8f, -0.4f, -1.0f, -0.8f, -0.4f, -0.5f, -0.8f, -0.7f, -0.9f, -0.8f, -0.4f, -0.5f, -0.8f, -0.7f, -0.9f, -0.8f, -0.7f, -0.6f, // Wheel 4

	    0.8f,-0.4f,-1.3f, -0.8f,-0.4f,-1.3f, 0.8f, 0.3f, -1.0f, -0.8f,-0.4f,-1.3f, 0.8f, 0.3f, -1.0f, -0.8f, 0.3f, -1.0f, // Lower Back
	    -0.8f,-0.4f, 1.4f, -0.8f,-0.4f,-1.3f, -0.8f, 0.3f, 1.0f, -0.8f,-0.4f,-1.3f, -0.8f, 0.3f, 1.0f, -0.8f, 0.3f, -1.0f, // Lower Right
	    0.8f,-0.4f, 1.4f, -0.8f,-0.4f, 1.4f, 0.8f,-0.4f,-1.3f, -0.8f,-0.4f, 1.4f, 0.8f,-0.4f,-1.3f, -0.8f,-0.4f,-1.3f, // Lower Below
	    -0.8f, 0.3f, 1.0f, 0.8f, 0.3f, 1.0f,  0.8f, 0.3f, -1.0f,  -0.8f, 0.3f, 1.0f,  0.8f, 0.3f, -1.0f,  -0.8f, 0.3f, -1.0f, // Lower Up
	    0.8f,-0.4f, 1.4f, -0.8f,-0.4f, 1.4f, 0.8f, 0.3f, 1.0f, -0.8f,-0.4f, 1.4f, 0.8f, 0.3f, 1.0f, -0.8f, 0.3f, 1.0f, // Lower Front
	    0.8f,-0.4f, 1.4f, 0.8f,-0.4f,-1.3f, 0.8f, 0.3f, 1.0f, 0.8f,-0.4f,-1.3f, 0.8f, 0.3f, 1.0f, 0.8f, 0.3f, -1.0f, // Lower Left

	    0.8f, 0.3f,-1.0f, -0.8f, 0.3f,-1.0f, 0.8f, 1.0f, -0.75f, -0.8f, 0.3f,-1.0f, 0.8f, 1.0f, -0.75f,  -0.8f, 1.0f, -0.75f, // Upper Back
	    -0.8f, 0.3f, 0.5f, -0.8f, 0.3f,-1.0f, -0.8f, 1.0f, 0.3f, -0.8f, 0.3f,-1.0f, -0.8f, 1.0f, 0.3f, -0.8f, 1.0f, -0.75f, // Upper Right
	    0.8f, 0.3f, 0.5f, -0.8f, 0.3f, 0.5f, 0.8f, 0.3f,-1.0f, -0.8f, 0.3f, 0.5f, 0.8f, 0.3f,-1.0f, -0.8f, 0.3f,-1.0f,  // Upper Below
	    0.8f, 0.3f, 0.5f, -0.8f, 0.3f, 0.5f, 0.8f, 1.0f, 0.3f, -0.8f, 0.3f, 0.5f, 0.8f, 1.0f, 0.3f, -0.8f, 1.0f, 0.3f, // Upper Front
	    0.8f, 0.3f, 0.5f, 0.8f, 0.3f,-1.0f, 0.8f, 1.0f, 0.3f, 0.8f, 0.3f,-1.0f, 0.8f, 1.0f, 0.3f, 0.8f, 1.0f, -0.75f, // Upper Left
	    0.8f, 1.0f, 0.3f, -0.8f, 1.0f, 0.3f, 0.8f, 1.0f, -0.75f, -0.8f, 1.0f, 0.3f, 0.8f, 1.0f, -0.75f, -0.8f, 1.0f, -0.75f // Upper Up
};

GLfloat texture_data[] {
		0.0f, 0.0f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.125f, // Wheel 1
	    0.0f, 0.0f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.125f, // Wheel 2
	    0.0f, 0.0f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.125f, // Wheel 3
	    0.0f, 0.0f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.0f, 0.0f, 0.125f, 0.162f, 0.125f, // Wheel 4
	    
	    // Lower Body
	    0.0f, 1.0f-0.0f,        0.363f, 1.0f-0.0f,      0.0f, 1.0f-0.21f,       0.363f, 1.0f,           0.0f, 1.0f-0.21f,   0.363f, 1.0f-0.21f, // Back
	    0.0f, 1.0f-0.505f,      0.73f, 1.0f-0.505f,     0.142f, 1.0f-0.753f,      0.73f, 1.0f-0.505f,     0.142f, 1.0f-0.753f,  0.625f, 1.0f-0.753f, // Rear Right
	    0.751f, 1.0f-0.531f,    1.0f, 1.0f-0.531f,      0.751f, 0.0f,           1.0f, 1.0f-0.531f,      0.751f, 0.0f,       1.0f, 0.0f, // Below
	    0.732f, 1.0f-0.213f,    0.365f, 1.0f-0.213f,    0.365f, 1.0f-0.46f,     0.732f, 1.0f-0.213f,    0.365f, 0.46f,      0.732f, 1.0f-0.46f, // Top
	    0.732f, 1.0f-0.0f,      0.365f, 1.0f,           0.732f, 1.0f-0.213f,    0.365f, 1.0f,           0.732f, 1.0f-0.213f, 0.365f, 1.0f-0.213f, // Front
	    0.0f, 1.0f-0.505f,      0.73f, 1.0f-0.505f,     0.142f, 1.0f-0.753f,      0.73f, 1.0f-0.505f,     0.142f, 1.0f-0.753f,  0.625f, 1.0f-0.753f, // Rear Left

	    // // Upper Body
	    0.0f, 1.0f-0.213f,       0.363f, 1.0f-0.213f,     0.0f, 1.0f-0.46f,     0.363f, 1.0f-0.213f, 0.0f, 1.0f-0.46f,    0.363f, 1.0f-0.46f, // Back
	    0.261f, 1.0f-0.755f,     0.73f, 1.0f-0.755f,      0.261f, 0.0f,         0.73f, 1.0f-0.755f,  0.261f, 0.0f,        0.73f, 0.0f, // Rear Right
	    0.751f, 1.0f-0.531f,     1.0f, 1.0f-0.531f,       0.751f, 0.0f,         1.0f, 1.0f-0.531f,   0.751f, 0.0f,        1.0f, 0.0f, // Below
	    0.0f, 1.0f-0.213f,       0.363f, 1.0f-0.213f,     0.0f, 1.0f-0.46f,     0.363f, 1.0f-0.213f, 0.0f, 1.0f-0.46f,    0.363f, 1.0f-0.46f, // Front
	    0.261f, 1.0f-0.755f,     0.73f, 1.0f-0.755f,      0.261f, 0.0f,         0.73f, 1.0f-0.755f,  0.261f, 0.0f,        0.73f, 0.0f, // Rear Left
		0.76f, 1.0f-0.531f,      1.0f, 1.0f-0.531f,       0.76f, 0.0f,          1.0f, 1.0f-0.531f,   0.76f, 0.0f,         1.0f, 0.0f // Top
};

// GLfloat particle_buffer_data[] = { // g_vertex_buffer_data
// 		-0.5f, -0.5f, 0.0f,
// 		0.5f, -0.5f, 0.0f,
// 		-0.5f, 0.5f, 0.0f,
// 		0.5f, 0.5f, 0.0f
// };

static GLfloat* g_particule_position_size_data = new GLfloat[PARTICLE_MAX_NUMBER * 4];;
static GLubyte* g_particule_color_data = new GLubyte[PARTICLE_MAX_NUMBER * 4];
static const GLfloat g_vertex_buffer_data[] = { 
	 -0.5f, -0.5f, 0.0f,
	  0.5f, -0.5f, 0.0f,
	 -0.5f,  0.5f, 0.0f,
	  0.5f,  0.5f, 0.0f,
};

#endif