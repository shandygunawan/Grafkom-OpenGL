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
	void setLightColor(glm::vec3 color);
};

LightController::LightController(){
	lightPos = glm::vec3(1.2, 1.0, 2.0);
}

void LightController::setShaderID(GLuint id){
	shaderID = id;
	setLightID();
}

void LightController::setLightID(){
	lightID = glGetUniformLocation(shaderID, "lightColor");
}


void LightController::setLightColor(glm::vec3 color){
	glUniform3fv(lightID, 1, glm::value_ptr(color));
}

#endif