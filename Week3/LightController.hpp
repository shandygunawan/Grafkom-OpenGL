#ifndef _LIGHTCONTROLLER_
#define _LIGHTCONTROLLER_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlobalLib.h"

using namespace std;

class LightController {
private:
	glm::vec3 lightPos;
	GLuint shaderID;
	GLuint lightID;

public:
	LightController();

	void setShaderID(GLuint id);
	void setLightID();
	void setLight();
};

LightController::LightController(){
	lightPos = glm::vec3(1.2, 1.0, 2.0);
}

void LightController::setShaderID(GLuint id){
	shaderID = id;
	setLightID();
}

void LightController::setLightID(){
	lightID = glGetUniformLocation(shaderID, "lightPos");
}


void LightController::setLight(){
	glUniform3fv(lightID, 1, GL_FALSE);
}

#endif