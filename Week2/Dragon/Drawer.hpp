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

using namespace std;

class Drawer{
private:
	RGB color_background;
	vector<vector<GLfloat>> vertices_buffer_data;
	vector<GLuint> vbos;
	IOController ioController;
	Utils utils;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();
	~Drawer();

	// Getter
	RGB getColorBackground();
	vector<GLuint> getVBOs();

	// Setter
	void setColorBackground(RGB color);

	// VBO
	void createVBOs();

	// Creator
	void createVerticesFromFile(string filename);
	
	// Drawer
	void drawTriangles();
	void drawImages();

	// Other
	void clearBackground();
	void printVertices(GLfloat vertices[], int size);
};

/* =============================================
	Constructor, Destructor, Copy Constructor
============================================== */
Drawer::Drawer(){
	// Set background's color
	color_background = {0.0f, 0.4f, 0.0f, 0.0f}; // Black
}

Drawer::~Drawer(){
	for(unsigned int i = 0; i < vbos.size(); i++){
		glDeleteBuffers(1, &vbos.at(i));
	}
}

/* =============================================
					Getter
============================================== */
RGB Drawer::getColorBackground(){
	return color_background;
}

vector<GLuint> Drawer::getVBOs(){
	return vbos;
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
void Drawer::createVBOs(){
	createVerticesFromFile(APP_INPUT_TEXT);

	for(unsigned int i = 0; i < vertices_buffer_data.size(); i++){
		/*
		printVertices(vertices_buffer_data.at(i).data(), vertices_buffer_data.at(i).size());
		cout << endl;
		cout << endl;
		*/
		
		GLuint vbo;
		glGenBuffers(1, &vbo);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    GLfloat *temp = vertices_buffer_data.at(i).data();
	    GLfloat data[vertices_buffer_data.at(i).size()];

	    for(unsigned int j = 0; j < vertices_buffer_data.at(i).size(); j++){
	    	data[j] = temp[j];
	    }
	    
	    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	    vbos.push_back(vbo);
	    
	}
}

/* =============================================
					Creator
============================================== */
void Drawer::createVerticesFromFile(string filename){
	vector<string> strings = ioController.readFile(filename);

	for(auto line = strings.begin(); line != strings.end(); line++){

		if(*line != APP_INPUT_SEPARATOR){
			vector<string> vertices_string = utils.explodeString(*line, ' ');
	        vector<GLfloat> vertices_float;
	        for(unsigned int i = 0; i < vertices_string.size(); i++){
	        	vertices_float.push_back(stof(vertices_string.at(i)));
	        }

	        vertices_buffer_data.push_back(vertices_float);	
		}
    }
}


/* =============================================
					Creator
============================================== */
void Drawer::drawImages(){
	for(unsigned int i = 0; i < vbos.size(); i++){

		glEnableVertexAttribArray(i);
        glBindBuffer(GL_ARRAY_BUFFER, vbos.at(i));
        glVertexAttribPointer(
			i,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
        );
	}

	int total_vertices_count = 0;

	for(unsigned int i = 0; i < vertices_buffer_data.size(); i++){
		total_vertices_count += ((vertices_buffer_data.at(i).size())/3);
	}

	glDrawArrays(GL_TRIANGLES, 0, total_vertices_count); // 5 triangles : 5*3 vertices
		
	for(unsigned int i = 0; i < vbos.size(); i++){
		glDisableVertexAttribArray(i);
	}

}

/* =============================================
					Other
============================================== */
void Drawer::clearBackground(){
	// Clear the screen.
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
}

void Drawer::printVertices(GLfloat vertices[], int size){

	for(int i = 0; i < size ; i++){
		
		if(i % 3 == 0 && i != 0){
			cout << endl;
		}

		cout << vertices[i] << ", ";
	}
}

#endif