#ifndef _APPCONTROLLER_
#define _APPCONTROLLER_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include "GlobalLib.h"
#include "CameraController.hpp"
#include "LightController.hpp"
#include "Drawer.hpp"

using namespace std;

class AppController{
private:
	// Window
	GLFWwindow* window;

	// Camera
	CameraController cameraController;

	// Lights
	// LightController lightController;

	// Model
	Model model;

	// Model Drawer
	Drawer drawer;

public:
	// Contructor, Destructor, Copy Constructor
	AppController();
	~AppController();

	// Setup
	int init();
	void close();
	void run();

	// VAO & VBO
	void createVAO();
	
};


/* =============================================
	Constructor, Destructor, Copy Constructor
============================================== */
AppController::AppController(){
	init();
}

AppController::~AppController(){
	close();
}


/* =============================================
					SETUP
============================================== */
int AppController::init(){

	// Initialize GLFW
	if( !glfwInit() )
	{
		fprintf( stderr, "Failed to initialize GLFW\n" );
		getchar();
		return -1;
	}

	// Initialize OpenGL Version & Profile
	glfwWindowHint(GLFW_SAMPLES, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	// Open a window and create its OpenGL context
	window = glfwCreateWindow(APP_WIDTH, APP_HEIGHT, APP_NAME, NULL, NULL);
	if( window == NULL ){
		fprintf(stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible.\n" );
		getchar();
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	// Initialize GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS); 
	
	// Set Background Color
	drawer.setColorBackground(drawer.getColorBackground());

	// Create VAO & VBOs
	drawer.loadModel(model);
	drawer.createVAO();
	drawer.createVBOs();

	// Load shaders
    drawer.loadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader" );

    // Create texture
    drawer.createTexture();

    // Setup Camera with its controller
    cameraController.setWindow(window);
    cameraController.setShaderID(drawer.getShaderID());

    // Setup Light
    // lightController.setShaderID(drawer.getShaderID());
    // lightController.setLight();

    return 0;
}

// Destroy/Disable VAO, VBO, shader, etc
void AppController::close(){

	// Destroy VAO, VBOs, Shaders
	drawer.destroy();

	// Close OpenGL Window and terminate GLFW
	glfwTerminate();
}

// Run the app
void AppController::run(){

	do{
		// Clear the screen.
		drawer.clearBackground();

        // Use our shader
        glUseProgram(drawer.getShaderID());

        cameraController.controlView();

        glBindVertexArray(drawer.getVAO());
        
        // Draw Images to screen
        drawer.drawModels();
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
}

#endif