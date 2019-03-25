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
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalLib.h"
#include "Drawer.hpp"

using namespace std;

class AppController{
private:
	GLFWwindow* window;
	Drawer drawer;

	GLuint VertexArrayID; // VAO
	GLuint shaderID; // Shader

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
	// void createVBO();

	// Shaders
	GLuint LoadShaders(const char * vertex_file_path,const char * fragment_file_path);
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

	// Initiliazie GLEW
	if (glewInit() != GLEW_OK) {
		fprintf(stderr, "Failed to initialize GLEW\n");
		getchar();
		glfwTerminate();
		return -1;
	}

	// Ensure we can capture the escape key being pressed below
	glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);

	// Set Background Color
	drawer.setColorBackground(drawer.getColorBackground());

	// Create VAO
	createVAO();

	// Create VBO
	drawer.createVBOs();

	vector<GLuint> vbo = drawer.getVerticesVBOs();
	for(unsigned int i = 0; i < vbo.size(); i++){
		cout << "VBO : " << i << endl;
		cout << "Value : " << vbo.at(i) << endl;	
	}
	cout << "============" << endl;
	vbo = drawer.getColorVBOs();
	for(unsigned int i = 0; i < vbo.size(); i++){
		cout << "VBO : " << i << endl;
		cout << "Value : " << vbo.at(i) << endl;	
	}

	// Create and compile our GLSL program from the shaders
    shaderID = LoadShaders( "vertexshader.vert", "fragmentshader.frag" );

    if(shaderID == NULL){
    	fprintf(stderr, "Failed to load shaders\n");
		getchar();
		glfwTerminate();
		return -1;
    }

    return 0;
}

// Destroy/Disable VAO, VBO, shader, etc
void AppController::close(){

	// Clean up VAO, VBO, and shader
	glDeleteProgram(shaderID);
	glDeleteVertexArrays(1, &VertexArrayID);

	// Close OpenGL Window and terminate GLFW
	glfwTerminate();
}

// Run the app
void AppController::run(){

	do{
		// Clear the screen.
		drawer.clearBackground();

        // Use our shader
        glUseProgram(shaderID);

        drawer.drawImages();
		
		// Swap buffers
		glfwSwapBuffers(window);
		glfwPollEvents();

	} // Check if the ESC key was pressed or the window was closed
	while( glfwGetKey(window, GLFW_KEY_ESCAPE ) != GLFW_PRESS &&
		   glfwWindowShouldClose(window) == 0 );
}

/* =============================================
					VAO & VBO
============================================== */
void AppController::createVAO(){
    glGenVertexArrays(1, &VertexArrayID);
    glBindVertexArray(VertexArrayID);
}


/* =============================================
					SHADERS
============================================== */
GLuint AppController::LoadShaders(const char * vertex_file_path,const char * fragment_file_path){

	// Create the shaders
	GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

	// Read the Vertex Shader code from the file
	std::string VertexShaderCode;
	std::ifstream VertexShaderStream(vertex_file_path, std::ios::in);
	if(VertexShaderStream.is_open()){
		std::stringstream sstr;
		sstr << VertexShaderStream.rdbuf();
		VertexShaderCode = sstr.str();
		VertexShaderStream.close();
	}else{
		printf("Impossible to open %s. Are you in the right directory ? Don't forget to read the FAQ !\n", vertex_file_path);
		getchar();
		return 0;
	}

	// Read the Fragment Shader code from the file
	std::string FragmentShaderCode;
	std::ifstream FragmentShaderStream(fragment_file_path, std::ios::in);
	if(FragmentShaderStream.is_open()){
		std::stringstream sstr;
		sstr << FragmentShaderStream.rdbuf();
		FragmentShaderCode = sstr.str();
		FragmentShaderStream.close();
	}

	GLint Result = GL_FALSE;
	int InfoLogLength;


	// Compile Vertex Shader
	printf("Compiling shader : %s\n", vertex_file_path);
	char const * VertexSourcePointer = VertexShaderCode.c_str();
	glShaderSource(VertexShaderID, 1, &VertexSourcePointer , NULL);
	glCompileShader(VertexShaderID);

	// Check Vertex Shader
	glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> VertexShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
		printf("%s\n", &VertexShaderErrorMessage[0]);
	}



	// Compile Fragment Shader
	printf("Compiling shader : %s\n", fragment_file_path);
	char const * FragmentSourcePointer = FragmentShaderCode.c_str();
	glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer , NULL);
	glCompileShader(FragmentShaderID);

	// Check Fragment Shader
	glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
	glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> FragmentShaderErrorMessage(InfoLogLength+1);
		glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
		printf("%s\n", &FragmentShaderErrorMessage[0]);
	}



	// Link the program
	printf("Linking program\n");
	GLuint ProgramID = glCreateProgram();
	glAttachShader(ProgramID, VertexShaderID);
	glAttachShader(ProgramID, FragmentShaderID);
	glLinkProgram(ProgramID);

	// Check the program
	glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
	glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
	if ( InfoLogLength > 0 ){
		std::vector<char> ProgramErrorMessage(InfoLogLength+1);
		glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
		printf("%s\n", &ProgramErrorMessage[0]);
	}

	
	glDetachShader(ProgramID, VertexShaderID);
	glDetachShader(ProgramID, FragmentShaderID);
	
	glDeleteShader(VertexShaderID);
	glDeleteShader(FragmentShaderID);

	return ProgramID;
}

#endif