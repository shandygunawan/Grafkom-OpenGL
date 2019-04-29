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
#include "Utils.hpp"
#include "Model.hpp"
// #include "ParticleController.hpp"

using namespace std;

class Drawer{
private:
	// VAO & VBO
	GLuint VAO;
	GLuint model_vbo;
	
	// Particle
	// ParticleController particleController;

	// Shader & Textures
	GLuint shaderID;
	GLuint textureID;

	// Model
	Model model;
	int triangleCount = 0;

	// Others
	RGB color_background;
	Utils utils;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();

	// Lifecycle
	void setup();
	void destroy();

	// Getter
	RGB getColorBackground();
	GLuint getVAO();
	GLuint getModelVBO();
	GLuint getTextureID();
	GLuint getShaderID();

	// Setter
	void setColorBackground(RGB color);
	void setModel(Model other);

	// Buffer Data
	vector<GLfloat> createBufferDataFromFiles(vector<string> filenames);

	// Creator
	void createVAO();
	void createModel();
	void createTexture();

	// Drawer
	void drawModel();
	void drawParticles();

	// Other
	void clearBackground();
	void printVertices(vector<GLfloat> vertices, int size);

	// Loader
	void loadBMP_glfw(char* imagepath);
	void loadShaders(const char * vertex_file_path,const char * fragment_file_path);
};

/* =============================================
	Constructor, Destructor, Copy Constructor
============================================== */
Drawer::Drawer(){
	// Set background's color
	color_background = {0.0f, 0.0f, 0.4f, 0.0f}; // Dark Blue
}

/* =============================================
					LIFECYCLE
============================================== */
void Drawer::setup(){
	loadShaders("VertexShader.vertexshader", "FragmentShader.fragmentshader");
	createVAO();
	createModel();
	createTexture();
	setColorBackground(getColorBackground());
}

void Drawer::destroy(){
	
	// Clean up VAO, VBO, and shader
	glDeleteProgram(shaderID);
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &model_vbo);
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

GLuint Drawer::getModelVBO(){
	return model_vbo;
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

void Drawer::setModel(Model other){
	model = other;
}

/* =============================================
					  CREATOR
============================================== */
void Drawer::createVAO(){
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
}

void Drawer::createModel(){

	// Generate Buffers
	glGenBuffers(1, &model_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo);

	triangleCount += (sizeof(model_vbo_data)/sizeof(GLfloat))/3;

    // Bind create buffer data
    glBufferData(GL_ARRAY_BUFFER, sizeof(model_vbo_data), model_vbo_data, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
    glVertexAttribPointer(
		LOCATION_MODEL,     
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
    );
}

void Drawer::createTexture(){
	loadBMP_glfw(model.getImage());

	glGenBuffers(1, &textureID);
    glBindBuffer(GL_ARRAY_BUFFER, textureID);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texture_data), texture_data, GL_STATIC_DRAW);
    glVertexAttribPointer(LOCATION_TEXTURE, 2, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(1);
}


/* =============================================
					DRAWER
============================================== */
void Drawer::drawModel(){

	glDrawArrays(GL_TRIANGLES, 0, triangleCount);

}

void Drawer::drawParticles(){

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


/* =============================================
					LOADER
============================================== */
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


#endif