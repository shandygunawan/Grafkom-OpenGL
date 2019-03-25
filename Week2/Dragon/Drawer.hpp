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
	vector<vector<GLfloat>> vertices_buffer_data;
	vector<vector<GLfloat>> color_buffer_data;
	vector<GLuint> vertices_vbos;
	vector<GLuint> color_vbos;
	IOController ioController;
	Utils utils;
	Image image;

public:
	// Contructor, Destructor, Copy Constructor
	Drawer();
	~Drawer();

	// Getter
	RGB getColorBackground();
	vector<GLuint> getVerticesVBOs();
	vector<GLuint> getColorVBOs();

	// Setter
	void setColorBackground(RGB color);

	// VBO
	void createVBOs();

	// Creator
	vector<GLfloat> createVerticesFromFile(string filename);
	vector<vector<GLfloat>> createVerticesFromFiles(vector<string> filenames);
	
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
	color_background = {1.0f, 1.0f, 1.0f, 1.0f}; // White
}

Drawer::~Drawer(){
	for(unsigned int i = 0; i < vertices_vbos.size(); i++){
		glDeleteBuffers(1, &vertices_vbos.at(i));
	}
	for(unsigned int i = 0; i < color_vbos.size(); i++){
		glDeleteBuffers(1, &color_vbos.at(i));
	}
}

/* =============================================
					Getter
============================================== */
RGB Drawer::getColorBackground(){
	return color_background;
}

vector<GLuint> Drawer::getVerticesVBOs(){
	return vertices_vbos;
}

vector<GLuint> Drawer::getColorVBOs(){
	return color_vbos;
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
	vertices_buffer_data = createVerticesFromFiles(image.getVertices());
	color_buffer_data = createVerticesFromFiles(image.getColors());

	for(unsigned int i = 0; i < vertices_buffer_data.size(); i++){
		GLuint vbo;
		glGenBuffers(1, &vbo);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    GLfloat *temp = vertices_buffer_data.at(i).data();
	    GLfloat data[vertices_buffer_data.at(i).size()];

		float widthMultiplier = ((float)APP_HEIGHT / (float)APP_WIDTH) * ((float)GRID_SIZE / (float)APP_WIDTH);
		float heightMultiplier = ((float)APP_WIDTH / (float)APP_HEIGHT) * ((float)GRID_SIZE / (float)APP_HEIGHT);
		float depthMultiplier = 1.0f; // Gak yakin harusnya berapa

	    for(unsigned int j = 0; j < vertices_buffer_data.at(i).size(); j+=3){
			float widthValue = temp[j+0] * widthMultiplier;
			float heightValue = temp[j+1] * heightMultiplier;
			float depthValue = temp[j+2] * depthMultiplier;

			cout << widthValue << " " << heightValue << " " << depthValue << endl;

			data[j] = widthValue;
			data[j+1] = heightValue;
			data[j+2] = depthValue;
	    }
	    
	    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	    vertices_vbos.push_back(vbo);
	}

	for(unsigned int i = 0; i < color_buffer_data.size(); i++){
		GLuint vbo;
		glGenBuffers(1, &vbo);
	    glBindBuffer(GL_ARRAY_BUFFER, vbo);
	    GLfloat *temp = color_buffer_data.at(i).data();
	    GLfloat data[color_buffer_data.at(i).size()];

	    for(unsigned int j = 0; j < color_buffer_data.at(i).size(); j++){
	    	data[j] = temp[j];
	    }
	    
	    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

	    color_vbos.push_back(vbo);
	}
}

/* =============================================
					Creator
============================================== */
vector<GLfloat> Drawer::createVerticesFromFile(string filename){
	vector<GLfloat> toReturn;
	vector<string> strings = ioController.readFile(filename);
	vector<string> vertices_string;
	vector<GLfloat> vertices_float;

	for(auto line = strings.begin(); line != strings.end(); line++){
		if(*line == APP_INPUT_SEPARATOR){
			for(unsigned int i = 0; i < vertices_string.size(); i++){
	        	toReturn.push_back(stof(vertices_string.at(i)));
	        }

			vertices_buffer_data.push_back(vertices_float);
			vertices_string.clear();
			vertices_float.clear();
		}
		else {
			vector<string> vertices_line = utils.explodeString(*line, ' ');

			for(unsigned int i = 0; i < vertices_line.size(); i++){
				vertices_string.push_back(vertices_line.at(i));
			}
		}
    }

    return toReturn;
}

vector<vector<GLfloat>> Drawer::createVerticesFromFiles(vector<string> filenames){
	vector<vector<GLfloat>> toReturn;
	for(unsigned int i = 0; i < filenames.size(); i++){
		toReturn.push_back(createVerticesFromFile(filenames.at(i)));
	}

	return toReturn;
}


/* =============================================
					Creator
============================================== */
void Drawer::drawImages(){
	// Draw shapes
	for(unsigned int i = 0; i < vertices_vbos.size()-1; i++){
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, vertices_vbos.at(i));
        glVertexAttribPointer(
			0,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
			3,                  // size
			GL_FLOAT,           // type
			GL_FALSE,           // normalized?
			0,                  // stride
			(void*)0            // array buffer offset
        );
	}

	
	glEnableVertexAttribArray(1);
	for(unsigned int i = 0; i < color_vbos.size(); i++){
        glBindBuffer(GL_ARRAY_BUFFER, color_vbos.at(i));
        glVertexAttribPointer(
			1,                  // attribute 0. No particular reason for 0, but must match the layout in the shader.
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

	for(unsigned int i = 0; i < color_buffer_data.size(); i++){
		total_vertices_count += ((color_buffer_data.at(i).size())/3);
	}

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
}

#endif