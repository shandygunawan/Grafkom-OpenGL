#ifndef _DRAWER_
#define _DRAWER_

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalLib.h"
#include "IOController.hpp"
#include "Utils.hpp"
#include "Model.hpp"

using namespace std;

class Drawer{
private:
	// VAO & VBO
	GLuint VAO;
	GLuint vertices_vbo;
	
	// Shader & Textures
	GLuint shaderID;
	GLuint textureID;

	// Controllers
	IOController ioController;

	// Model
	Model model;
	int triangleCount = 0;

	// Others
	RGB color_background;
	Utils utils;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();

	// Destroyer
	void destroy();

	// Getter
	RGB getColorBackground();
	GLuint getVAO();
	GLuint getVerticesVBO();
	GLuint getTextureID();
	GLuint getShaderID();

	// Setter
	void setColorBackground(RGB color);

	// Loader
	void loadModel(Model other);
	void loadBMP_glfw(char* imagepath);
	void loadShaders(const char * vertex_file_path,const char * fragment_file_path);

	// Buffer Data
	vector<GLfloat> createBufferDataFromFiles(vector<string> filenames);

	// VAO & VBO
	void createVAO();
	void createVBOs();
	void bindVBOs();

	// Texture
	void createTexture();

	// Drawer
	void drawModels();

	// Other
	void clearBackground();
	void printVertices(vector<GLfloat> vertices, int size);
};

/* =============================================
	Constructor, Destructor, Copy Constructor
============================================== */
Drawer::Drawer(){
	// Set background's color
	color_background = {0.0f, 0.0f, 0.4f, 0.0f}; // Dark Blue
}

/* =============================================
					DESTROYER
============================================== */
void Drawer::destroy(){
	
	// Clean up VAO, VBO, and shader
	glDeleteProgram(shaderID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &vertices_vbo);
}

/* =============================================
					Getter
============================================== */
RGB Drawer::getColorBackground(){
	return color_background;
}

GLuint Drawer::getVAO(){
	return VAO;
}

GLuint Drawer::getVerticesVBO(){
	return vertices_vbo;
}

GLuint Drawer::getTextureID(){
	return textureID;
}

GLuint Drawer::getShaderID(){
	return shaderID;
}

/* =============================================
					Setter
============================================== */
void Drawer::setColorBackground(RGB color){
	color_background.R = color.R;
	color_background.G = color.G;
	color_background.B = color.B;
	color_background.A = color.A;
	glClearColor(color_background.R, color_background.G, color_background.B, color_background.A);
}

/* =============================================
					LOADER
============================================== */
void Drawer::loadModel(Model other){
	model = other;
}

void Drawer::loadBMP_glfw(char* imagepath) {
    glGenTextures(1, &textureID);
    
    glBindTexture(GL_TEXTURE_2D, textureID);

    // load and generate the texture
    int width, height, nrChannels;
    unsigned char *data = stbi_load(imagepath, &width, &height, &nrChannels, 0);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    stbi_image_free(data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
}

void Drawer::loadShaders(const char * vertex_file_path,const char * fragment_file_path){

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

	shaderID = ProgramID;
}

/* =============================================
					Buffer Data
============================================== */
vector<GLfloat> Drawer::createBufferDataFromFiles(vector<string> filenames){
	vector<GLfloat> toReturn;

	for(auto filename = filenames.begin(); filename != filenames.end(); filename++){
		vector<string> strings = ioController.readFile(*filename);
		
		for(auto line = strings.begin(); line != strings.end(); line++){
			vector<string> vertices_line = utils.explodeString(*line, ' ');

			if(vertices_line.at(0) != "#"){	
				for(unsigned int i = 0; i < vertices_line.size(); i++){
					toReturn.push_back(stof(vertices_line.at(i)));
				}	
			}
			
	    }
	}

	printVertices(toReturn, toReturn.size());

    return toReturn;
}

/* =============================================
					VAO &  VBO
============================================== */
void Drawer::createVAO(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void Drawer::createVBOs(){
	// vertices_buffer_data = createBufferDataFromFiles(model.getVertices());

	// Generate Buffers
	glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);

    GLfloat vertices_vbo_data[] {
		
		-1.47, -0.71, 0.97, -1.47, -1.11, 0.97, -1.07, -1.11, 0.97,
		-1.47, -0.71, 0.97, -1.07, -0.71, 0.97, -1.07, -1.11, 0.97,
		-1.47, -0.71, 0.67, -1.47, -1.11, 0.67, -1.07, -1.11, 0.67,
		-1.47, -0.71, 0.67, -1.07, -0.71, 0.67, -1.07, -1.11, 0.67,
		-1.47, -0.71, 0.67, -1.47, -0.71, 0.97, -1.07, -0.71, 0.97,
		-1.47, -0.71, 0.67, -1.07, -0.71, 0.67, -1.07, -0.71, 0.97,
		-1.47, -1.11, 0.67, -1.47, -1.11, 0.97, -1.07, -1.11, 0.97,
		-1.47, -1.11, 0.67, -1.07, -1.11, 0.67, -1.07, -1.11, 0.97,
		-1.47, -0.71, 0.97, -1.47, -0.71, 0.67, -1.47, -1.11, 0.67,
		-1.47, -0.71, 0.97, -1.47, -1.11, 0.97, -1.47, -1.11, 0.67,
		-1.07, -0.71, 0.97, -1.07, -0.71, 0.67, -1.07, -1.11, 0.67,
		-1.07, -0.71, 0.97, -1.07, -1.11, 0.97, -1.07, -1.11, 0.67,
		
		// Belakang kiri
		-1.47, -0.71, -0.67, -1.47, -1.11, -0.67, -1.07, -1.11, -0.67,
		-1.47, -0.71, -0.67, -1.07, -0.71, -0.67, -1.07, -1.11, -0.67,
		-1.47, -0.71, -0.97, -1.47, -1.11, -0.97, -1.07, -1.11, -0.97,
		-1.47, -0.71, -0.97, -1.07, -0.71, -0.97, -1.07, -1.11, -0.97,
		-1.47, -0.71, -0.97, -1.47, -0.71, -0.67, -1.07, -0.71, -0.67,
		-1.47, -0.71, -0.97, -1.07, -0.71, -0.97, -1.07, -0.71, -0.67,
		-1.47, -1.11, -0.97, -1.47, -1.11, -0.67, -1.07, -1.11, -0.67,
		-1.47, -1.11, -0.97, -1.07, -1.11, -0.97, -1.07, -1.11, -0.67,
		-1.47, -0.71, -0.67, -1.47, -0.71, -0.97, -1.47, -1.11, -0.97,
		-1.47, -0.71, -0.67, -1.47, -1.11, -0.67, -1.47, -1.11, -0.97,
		-1.07, -0.71, -0.67, -1.07, -0.71, -0.97, -1.07, -1.11, -0.97,
		-1.07, -0.71, -0.67, -1.07, -1.11, -0.67, -1.07, -1.11, -0.97,
		
		// Depan kanan
		0.97, -0.71, 0.97, 0.97, -1.11, 0.97, 1.47, -1.11, 0.97,
		0.97, -0.71, 0.97, 1.47, -0.71, 0.97, 1.47, -1.11, 0.97,
		0.97, -0.71, 0.67, 0.97, -1.11, 0.67, 1.47, -1.11, 0.67,
		0.97, -0.71, 0.67, 1.47, -0.71, 0.67, 1.47, -1.11, 0.67,
		0.97, -0.71, 0.67, 0.97, -0.71, 0.97, 1.47, -0.71, 0.97,
		0.97, -0.71, 0.67, 1.47, -0.71, 0.67, 1.47, -0.71, 0.97,
		0.97, -1.11, 0.67, 0.97, -1.11, 0.97, 1.47, -1.11, 0.97,
		0.97, -1.11, 0.67, 1.47, -1.11, 0.67, 1.47, -1.11, 0.97,
		0.97, -0.71, 0.97, 0.97, -0.71, 0.67, 0.97, -1.11, 0.67,
		0.97, -0.71, 0.97, 0.97, -1.11, 0.97, 0.97, -1.11, 0.67,
		1.47, -0.71, 0.97, 1.47, -0.71, 0.67, 1.47, -1.11, 0.67,
		1.47, -0.71, 0.97, 1.47, -1.11, 0.97, 1.47, -1.11, 0.67,
		
		// Depan kiri
		0.97, -0.71, -0.67, 0.97, -1.11, -0.67, 1.47, -1.11, -0.67,
		0.97, -0.71, -0.67, 1.47, -0.71, -0.67, 1.47, -1.11, -0.67,
		0.97, -0.71, -0.97, 0.97, -1.11, -0.97, 1.47, -1.11, -0.97,
		0.97, -0.71, -0.97, 1.47, -0.71, -0.97, 1.47, -1.11, -0.97,
		0.97, -0.71, -0.97, 0.97, -0.71, -0.67, 1.47, -0.71, -0.67,
		0.97, -0.71, -0.97, 1.47, -0.71, -0.97, 1.47, -0.71, -0.67,
		0.97, -1.11, -0.97, 0.97, -1.11, -0.67, 1.47, -1.11, -0.67,
		0.97, -1.11, -0.97, 1.47, -1.11, -0.97, 1.47, -1.11, -0.67,
		0.97, -0.71, -0.67, 0.97, -0.71, -0.97, 0.97, -1.11, -0.97,
		0.97, -0.71, -0.67, 0.97, -1.11, -0.67, 0.97, -1.11, -0.97,
		1.47, -0.71, -0.67, 1.47, -0.71, -0.97, 1.47, -1.11, -0.97,
		1.47, -0.71, -0.67, 1.47, -1.11, -0.67, 1.47, -1.11, -0.97,
		

		// BADAN MOBIL
		// Part 1
		-1.82, -0.86, -0.82, -1.82, 0.86, -0.82, -1.52, 0.86, -0.82,
		-1.82, -0.86, -0.82, -1.52, -0.86, -0.82, -1.52, 0.86, -0.82,
		-1.82, -0.86, -0.82, -1.82, -0.86, 0.82, -1.52, -0.86, 0.82,
		-1.82, -0.86, -0.82, -1.52, -0.86, -0.82, -1.52, -0.86, 0.82,
		-1.82, 0.86, -0.82, -1.82, 0.86, 0.82, -1.52, 0.86, 0.82,
		-1.82, 0.86, -0.82, -1.52, 0.86, -0.82, -1.52, 0.86, 0.82,
		-1.82, -0.86, 0.82, -1.82, -0.86, -0.82, -1.82, 0.86, -0.82,
		-1.82, -0.86, 0.82, -1.82, 0.86, 0.82, -1.82, 0.86, -0.82,
		-1.52, -0.86, 0.82, -1.52, -0.86, -0.82, -1.52, 0.86, -0.82,
		-1.52, -0.86, 0.82, -1.52, 0.86, 0.82, -1.52, 0.86, -0.82,
		// Part 1 sekitar roda
		-1.52, -0.86, 0.82, -1.52, -0.66, 0.82, -1.52, -0.66, 0.82,
		-1.52, -0.86, 0.82, -1.52, -0.86, 0.82, -1.52, -0.66, 0.82,
		
		// Part 2
		-1.52, -0.66, -0.82, -1.52, -0.66, 0.82, -1.02, -0.66, 0.82,
		-1.52, -0.66, -0.82, -1.02, -0.66, -0.82, -1.02, -0.66, 0.82,
		-1.52, 0.86, -0.82, -1.52, 0.86, 0.82, -1.02, 0.86, 0.82,
		-1.52, 0.86, -0.82, -1.02, 0.86, -0.82, -1.02, 0.86, 0.82,
		-1.52, -0.66, 0.82, -1.52, -0.66, -0.82, -1.52, 0.86, -0.82,
		-1.52, -0.66, 0.82, -1.52, 0.86, 0.82, -1.52, 0.86, -0.82,
		-1.02, -0.66, 0.82, -1.02, -0.66, -0.82, -1.02, 0.86, -0.82,
		-1.02, -0.66, 0.82, -1.02, 0.86, 0.82, -1.02, 0.86, -0.82,
		
		// Part 3.1
		-1.02, 0.20, 0.82, -1.02, 0.86, 0.82, 0.82, 0.86, 0.82,
		-1.02, 0.20, 0.82, 0.82, 0.20, 0.82, 0.82, 0.86, 0.82,
		-1.02, 0.20, -0.82, -1.02, 0.20, 0.82, 0.82, 0.20, 0.82,
		-1.02, 0.20, -0.82, 0.82, 0.20, -0.82, 0.82, 0.20, 0.82,
		-1.02, 0.20, 0.82, -1.02, 0.20, -0.82, -1.02, 0.86, -0.82,
		-1.02, 0.20, 0.82, -1.02, 0.86, 0.82, -1.02, 0.86, -0.82,
		0.82, 0.20, 0.82, 0.82, 0.20, -0.82, 0.82, 0.86, -0.82,
		0.82, 0.20, 0.82, 0.82, 0.86, 0.82, 0.82, 0.86, -0.82,
		
		// Part 3.2
		-1.02, 0.20, -0.82, -1.02, 0.20, 0.82, 0.82, 0.20, 0.82,
		-1.02, 0.20, -0.82, 0.82, 0.20, -0.82, 0.82, 0.20, 0.82,
		-1.02, -0.86, 0.82, -1.02, -0.86, -0.82, -1.02, 0.20, -0.82,
		-1.02, -0.86, 0.82, -1.02, 0.20, 0.82, -1.02, 0.20, -0.82,
		0.82, -0.86, 0.82, 0.82, -0.86, -0.82, 0.82, 0.20, -0.82,
		0.82, -0.86, 0.82, 0.82, 0.20, 0.82, 0.82, 0.20, -0.82,
		
		// Part 3 sekitar roda
		-1.02, -0.86, -0.82, -1.02, -0.66, -0.82, 0.82, -0.66, -0.82,
		-1.02, -0.86, -0.82, 0.82, -0.86, -0.82, 0.82, -0.66, -0.82,
		
		// Part 4
		0.82, 0.20, -0.82, 0.82, 0.20, 0.82, 1.02, 0.20, 0.82,
		0.82, 0.20, -0.82, 1.02, 0.20, -0.82, 1.02, 0.20, 0.82,
		0.82, -0.66, -0.82, 0.82, -0.66, 0.82, 1.02, -0.66, 0.82,
		0.82, -0.66, -0.82, 1.02, -0.66, -0.82, 1.02, -0.66, 0.82,
		0.82, 0.20, 0.82, 0.82, 0.20, -0.82, 0.82, -0.66, -0.82,
		0.82, 0.20, 0.82, 0.82, -0.66, 0.82, 0.82, -0.66, -0.82,
		1.02, 0.20, 0.82, 1.02, 0.20, -0.82, 1.02, -0.66, -0.82,
		1.02, 0.20, 0.82, 1.02, -0.66, 0.82, 1.02, -0.66, -0.82,
		
		// Part 4 sekitar roda
		0.82, 0.20, 0.82, 0.82, -0.66, 0.82, 1.02, -0.66, 0.82,
		0.82, 0.20, 0.82, 1.02, 0.20, 0.82, 1.02, -0.66, 0.82,
		
		// Part 5
		1.02, -0.66, -0.82, 1.02, -0.66, 0.82, 1.52, -0.66, 0.82,
		1.02, -0.66, -0.82, 1.52, -0.66, -0.82, 1.52, -0.66, 0.82,
		1.02, 0.20, -0.82, 1.02, 0.20, 0.82, 1.52, 0.20, 0.82,
		1.02, 0.20, -0.82, 1.52, 0.20, -0.82, 1.52, 0.20, 0.82,
		1.02, -0.66, 0.82, 1.02, -0.66, -0.82, 1.02, 0.20, -0.82,
		1.02, -0.66, 0.82, 1.02, 0.20, 0.82, 1.02, 0.20, -0.82,
		1.52, -0.66, 0.82, 1.52, -0.66, -0.82, 1.52, 0.20, -0.82,
		1.52, -0.66, 0.82, 1.52, 0.20, 0.82, 1.52, 0.20, -0.82,
		
		// Part 6
		1.52, -0.86, 0.82, 1.52, 0.20, 0.82, 1.82, 0.20, 0.82,
		1.52, -0.86, 0.82, 1.82, -0.86, 0.82, 1.82, 0.20, 0.82,
		1.52, -0.86, -0.82, 1.52, -0.86, 0.82, 1.82, -0.86, 0.82,
		1.52, -0.86, -0.82, 1.82, -0.86, -0.82, 1.82, -0.86, 0.82,
		1.52, 0.20, -0.82, 1.52, 0.20, 0.82, 1.82, 0.20, 0.82,
		1.52, 0.20, -0.82, 1.82, 0.20, -0.82, 1.82, 0.20, 0.82,
		1.52, -0.86, 0.82, 1.52, -0.86, -0.82, 1.52, 0.20, -0.82,
		1.52, -0.86, 0.82, 1.52, 0.20, 0.82, 1.52, 0.20, -0.82,
		1.82, -0.86, 0.82, 1.82, -0.86, -0.82, 1.82, 0.20, -0.82,
		1.82, -0.86, 0.82, 1.82, 0.20, 0.82, 1.82, 0.20, -0.82,
		
		// Part 6 sekitar roda
		1.52, -0.86, -0.82, 1.52, -0.66, -0.82, 1.82, -0.66, -0.82,
		1.52, -0.86, -0.82, 1.82, -0.86, -0.82, 1.82, -0.66, -0.82

		
		/*
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
		*/
	};
	triangleCount += (sizeof(vertices_vbo_data)/sizeof(GLfloat))/3;

    // Bind create buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_vbo_data), vertices_vbo_data, GL_STATIC_DRAW);

    bindVBOs();
}

void Drawer::bindVBOs(){
	// Draw vertices
	glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    glVertexAttribPointer(
		0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
    );
}


/* =============================================
					TEXTURE
============================================== */
void Drawer::createTexture(){
	loadBMP_glfw(model.getImage());

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

	glGenBuffers(1, &textureID);
    glBindBuffer(GL_ARRAY_BUFFER, textureID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_data), texture_data, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);
}


/* =============================================
					DRAWER
============================================== */
void Drawer::drawModels(){

	glDrawArrays(GL_TRIANGLES, 0, triangleCount);

}

/* =============================================
					Other
============================================== */
void Drawer::clearBackground(){
	// Clear the screen.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Drawer::printVertices(vector<GLfloat> vertices, int size){

	for(int i = 0; i < size ; i++){
		
		if(i % 3 == 0 && i != 0){
			cout << endl;
		}

		cout << vertices[i] << ", ";
	}

	cout << endl;
	cout << endl;
}

#endif