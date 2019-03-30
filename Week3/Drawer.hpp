#ifndef _DRAWER_
#define _DRAWER_

#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "GlobalLib.h"
#include "IOController.hpp"
#include "Utils.hpp"
#include "Image.hpp"

using namespace std;

class Drawer{
private:
	RGB color_background;
	GLuint vertices_vbo;
	GLuint color_vbo;
	vector<GLfloat> vertices_buffer_data;
	vector<GLfloat> color_buffer_data;
	IOController ioController;
	Utils utils;
	Image image;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();
	~Drawer();

	// Getter
	RGB getColorBackground();
	GLuint getVerticesVBO();
	GLuint getColorVBO();

	// Setter
	void setColorBackground(RGB color);

	// VBO
	void createVBOs(string type);

	// Creator
	vector<GLfloat> createBufferDataFromFiles(vector<string> filenames);
	// vector<GLfloat> createColorFromFiles(vector<string> filenames);
	
	// Drawer
	void drawTriangles();
	void drawImages();

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
	glDeleteBuffers(1, &color_vbo);
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

GLuint Drawer::getColorVBO(){
	return color_vbo;
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
	vertices_buffer_data = createBufferDataFromFiles(image.getVertices());
	color_buffer_data = createBufferDataFromFiles(image.getColors());

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


	// Create VBO for color
	glGenBuffers(1, &color_vbo);
    glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    GLfloat *color_vbo_temp = color_buffer_data.data();
    GLfloat color_vbo_data[color_buffer_data.size()];

    for(unsigned int j = 0; j < color_buffer_data.size(); j++){
    	color_vbo_data[j] = color_vbo_temp[j];
	}
	glBufferData(GL_ARRAY_BUFFER, sizeof(color_vbo_data), color_vbo_data, GL_STATIC_DRAW);
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

			for(unsigned int i = 0; i < vertices_line.size(); i++){
				toReturn.push_back(stof(vertices_line.at(i)));
			}
	    }
	}

	printVertices(toReturn, toReturn.size());

    return toReturn;
}

/*
vector<GLfloat> Drawer::createColorFromFiles(vector<string> filenames){
	vector<GLfloat> toReturn;

	for(unsigned int i = 0; i < filenames.size(); i++){
		vector<string> file_strings = ioController.readFile(filenames.at(i));

		for(auto line = file_strings.begin(); line != file_strings.end(); line++){
			vector<string> color_line = utils.explodeString(*line, ' ');

			for(unsigned int j = 0; j < color_line.size(); j++){
				toReturn.push_back(stof(color_line.at(j)));
			}
	    }
	}

	return toReturn;
}
*/

/* =============================================
					DRAW
============================================== */
void Drawer::drawImages(){
	
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

    glEnableVertexAttribArray(1);
	// printVertices(color_buffer_data, color_buffer_data.size());
	glBindBuffer(GL_ARRAY_BUFFER, color_vbo);
    glVertexAttribPointer(
		1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
		3,                  // size
		GL_FLOAT,           // type
		GL_FALSE,           // normalized?
		0,                  // stride
		(void*)0            // array buffer offset
    );

	int total_vertices_count = ((vertices_buffer_data.size() + color_buffer_data.size())/3);

	// cout << "total vertices count: " << total_vertices_count << endl;

	glDrawArrays(GL_TRIANGLES, 0, total_vertices_count); // 5 triangles : 5*3 vertices
		
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