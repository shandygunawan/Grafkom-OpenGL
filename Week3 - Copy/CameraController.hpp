#ifndef _INPUTCONTROLLER_
#define _INPUTCONTROLLER_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "GlobalLib.h"

class CameraController {
private:
	// CAMERA VIEW
	GLFWwindow* window;
	glm::mat4 ViewMatrix;
	glm::mat4 ProjectionMatrix;
	GLuint shaderID;
	GLuint matrixID;  // Get a handle for our "MVP" uniform
	glm::vec3 position = glm::vec3(5, 2.5, 5); // position
	float horizontalAngle = 3.14f; // Horizontal angle: toward-Z
	float verticalAngle = 0.0f; // Vertical angle: 0, look at the horizon
	float initialFoV = 45.0f; // Initial field of view


	// MOUSE & KEYBOARD
	float speed = 3.0f; // 3 units / second
	float mouseSpeed = 0.005f;

public:
	// Constructor, Destructor, Copy Constructor
	CameraController();

	// Setter
	void setWindow(GLFWwindow* inputWindow);
	void setShaderID(GLuint id);
	void setMatrixID();

	// Getter
	glm::mat4 getViewMatrix();
	glm::mat4 getProjectionMatrix();

	// Camera Control
	void controlView();

	// Inputs Handling
	void computeMatricesFromInputs();
};

/* =============================================
	Constructor, Destructor, Copy Constructor
============================================== */
CameraController::CameraController(){

}

/* =============================================
					SETTER
============================================== */
void CameraController::setWindow(GLFWwindow* inputWindow){
	window = inputWindow;
}

void CameraController::setShaderID(GLuint id){
	shaderID = id;
	setMatrixID();
}

void CameraController::setMatrixID(){
	matrixID = glGetUniformLocation(shaderID, "MVP");
}

/* =============================================
					GETTER
============================================== */
glm::mat4 CameraController::getViewMatrix(){
	return ViewMatrix;
}
glm::mat4 CameraController::getProjectionMatrix(){
	return ProjectionMatrix;
}


/* =============================================
				CAMERA CONTROL
============================================== */
void CameraController::controlView(){
	// Compute the MVP matrix from keyboard and mouse input
	computeMatricesFromInputs();
	glm::mat4 ProjectionMatrix = getProjectionMatrix();
	glm::mat4 ViewMatrix = getViewMatrix();
	glm::mat4 ModelMatrix = glm::mat4(1.0);
	glm::mat4 MVP = ProjectionMatrix * ViewMatrix * ModelMatrix;

	// Send our transformation to the currently bound shader, 
	// in the "MVP" uniform
	glUniformMatrix4fv(matrixID, 1, GL_FALSE, &MVP[0][0]);
}

/* =============================================
					INPUTS HANDLING
============================================== */
void CameraController::computeMatricesFromInputs(){

	// glfwGetTime is called only once, the first time this function is called
	static double lastTime = glfwGetTime();

	// Compute time difference between current and last frame
	double currentTime = glfwGetTime();
	float deltaTime = float(currentTime - lastTime);

	// Get mouse position
	double xpos, ypos;
	glfwGetCursorPos(window, &xpos, &ypos);

	// Reset mouse position for next frame
	glfwSetCursorPos(window, APP_WIDTH/2, APP_HEIGHT/2);

	// Compute new orientation
	horizontalAngle += mouseSpeed * float(APP_WIDTH/2 - xpos );
	verticalAngle   += mouseSpeed * float(APP_HEIGHT/2 - ypos );

	// Direction : Spherical coordinates to Cartesian coordinates conversion
	glm::vec3 direction(
		cos(verticalAngle) * sin(horizontalAngle), 
		sin(verticalAngle),
		cos(verticalAngle) * cos(horizontalAngle)
	);
	
	// Right vector
	glm::vec3 right = glm::vec3(
		sin(horizontalAngle - 3.14f/2.0f), 
		0,
		cos(horizontalAngle - 3.14f/2.0f)
	);
	
	// Up vector
	glm::vec3 up = glm::cross( right, direction );

	// Move forward
	if (glfwGetKey( window, GLFW_KEY_UP ) == GLFW_PRESS){
		position += direction * deltaTime * speed;
	}
	// Move backward
	if (glfwGetKey( window, GLFW_KEY_DOWN ) == GLFW_PRESS){
		position -= direction * deltaTime * speed;
	}
	// Strafe right
	if (glfwGetKey( window, GLFW_KEY_RIGHT ) == GLFW_PRESS){
		position += right * deltaTime * speed;
	}
	// Strafe left
	if (glfwGetKey( window, GLFW_KEY_LEFT ) == GLFW_PRESS){
		position -= right * deltaTime * speed;
	}

	float FoV = initialFoV;// - 5 * glfwGetMouseWheel(); // Now GLFW 3 requires setting up a callback for this. It's a bit too complicated for this beginner's tutorial, so it's disabled instead.

	// Projection matrix : 45° Field of View, 1:1 ratio, display range : 0.1 unit <-> 100 units
	ProjectionMatrix = glm::perspective(glm::radians(FoV), 1.0f / 1.0f, 0.1f, 100.0f);
	// Camera matrix
	ViewMatrix       = glm::lookAt(
								position,           // Camera is here
								position+direction, // and looks here : at the same position, plus "direction"
								up                  // Head is up (set to 0,-1,0 to look upside-down)
						   );

	// For the next frame, the "last time" will be "now"
	lastTime = currentTime;
}

#endif