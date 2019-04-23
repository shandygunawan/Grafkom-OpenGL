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
	RGB color_background;
	GLuint vertices_vbo;
	GLuint color_vbo;
	GLuint texture;
	GLuint shaderID;
	GLuint textureID;
	vector<GLfloat> vertices_buffer_data;
	vector<GLfloat> uv_buffer_data;
	IOController ioController;
	Utils utils;
	Model model;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();
	~Drawer();

	// Getter
	RGB getColorBackground();
	GLuint getVerticesVBO();
	GLuint getUvVBO();

	// Setter
	void setColorBackground(RGB color);

	// VBO
	void createVBOs(string type);
	void bindVBOs();

	// Texture
	void setShaderID(GLuint id);
	void setTextureID();
	void createTexture();

	// Creator
	vector<GLfloat> createBufferDataFromFiles(vector<string> filenames);

	// Drawer
	void drawModel();

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

Drawer::~Drawer(){
	glDeleteBuffers(1, &vertices_vbo);
	glDeleteBuffers(1, &uv_vbo);
	glDeleteBuffers(1, &texture);
}

/* =============================================
					Getter
============================================== */
RGB Drawer::getColorBackground(){
	return color_background;
}

GLuint Drawer::getVerticesVBO(){
	return vertices_vbo;
}

GLuint Drawer::getUvVBO(){
	return uv_vbo;
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
					  VBO
============================================== */
void Drawer::createVBOs(string type){
	vertices_buffer_data = createBufferDataFromFiles(model.getVertices());
	uv_buffer_data = createBufferDataFromFiles(model.getColors());

	// Create VBO for vertices
	glGenBuffers(1, &vertices_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vertices_vbo);
    GLfloat *vertices_vbo_temp = vertices_buffer_data.data();
    GLfloat vertices_vbo_data[vertices_buffer_data.size()];

    if(type == "grid"){
	    float widthMultiplier = (float)GRID_SIZE / (float)APP_WIDTH;
		float heightMultiplier = ((float)APP_WIDTH / (float)APP_HEIGHT) * widthMultiplier;
		float depthMultiplier = 1.0f; // Gak yakin harusnya berapa

	    for(unsigned int j = 0; j < vertices_buffer_data.size(); j+=3){
			float widthValue = vertices_vbo_temp[j+0] * widthMultiplier;
			float heightValue = vertices_vbo_temp[j+1] * heightMultiplier;
			float depthValue = vertices_vbo_temp[j+2] * depthMultiplier;

			// cout << widthValue << " " << heightValue << " " << depthValue << endl;

			vertices_vbo_data[j] = widthValue;
			vertices_vbo_data[j+1] = heightValue;
			vertices_vbo_data[j+2] = depthValue;
	    }
			
    }
    else if(type == "coord"){
    	for(unsigned int i = 0; i < vertices_buffer_data.size() ; i++){
	    	vertices_vbo_data[i] = vertices_vbo_temp[i];
	    }
    }
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices_vbo_data), vertices_vbo_data, GL_STATIC_DRAW);


	// Create VBO for VBO
    glGenBuffers(1, &uv_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    GLfloat *uv_vbo_temp = uv_buffer_data.data();
    GLfloat uv_vbo_data[uv_buffer_data.size()];

    for(unsigned int j = 0; j < uv_buffer_data.size(); j++){
    	uv_vbo_data[j] = uv_vbo_temp[j];
	}
	
	glBufferData(GL_ARRAY_BUFFER, sizeof(uv_vbo_data), uv_vbo_data, GL_STATIC_DRAW);

	bindVBOs();
}

void Drawer::bindVBOs(){
	// Bind vertices
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

    // Bind Colors
    glEnableVertexAttribArray(1);
	// printVertices(color_buffer_data, color_buffer_data.size());
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
    glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		2,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
    );
}

/* =============================================
					  TEXTURE
============================================== */
void Drawer::setShaderID(GLuint id){
	shaderID = id;
	setTextureID();
}

void Drawer::setTextureID(){
	textureID  = glGetUniformLocation(shaderID, "textureSampler");
}

void Drawer::createTexture(){
	texture = model.loadDDS("jimny.DDS");
}

/* =============================================
					Creator
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
					DRAW
============================================== */
void Drawer::drawModel(){

	// Bind our texture in Texture Unit 0
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	// Set our "textureSampler" sampler to use Texture Unit 0
	glUniform1i(textureID, 0);

	bindVBOs();

	int total_vertices_count = ((vertices_buffer_data.size() + uv_buffer_data.size())/3);
	glDrawArrays(GL_TRIANGLES, 0, total_vertices_count);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
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