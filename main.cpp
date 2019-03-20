#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}
int main(void)
{
    // Setting Window
    GLFWwindow* window;
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);
    window = glfwCreateWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Simple example", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
    glfwSetKeyCallback(window, key_callback);
    while (!glfwWindowShouldClose(window))
    {
        float ratio;
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        ratio = width / (float) height;
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        // glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);

        glBegin(GL_TRIANGLES);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.f);
        glVertex3f(0.5f, 0.0f, 0.f);
        glVertex3f(0.154508497187f, 0.475528258148f, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.f);
        glVertex3f(0.154508497187474f, -0.475528258147577f, 0.f);
        glVertex3f(0.5f, 0.0f, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.f);
        glVertex3f(-0.404508497187474f, -0.293892626146237, 0.f);
        glVertex3f(0.154508497187474f, -0.475528258147577f, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.f);
        glVertex3f(-0.404508497187474f, 0.293892626146237, 0.f);
        glVertex3f(-0.404508497187474f, -0.293892626146237, 0.f);
        glEnd();

        glBegin(GL_TRIANGLES);
        glColor3f(0.f, 0.f, 1.f);
        glVertex3f(0.0f, 0.0f, 0.f);
        glVertex3f(0.154508497187474f, 0.475528258147577f, 0.f);
        glVertex3f(-0.404508497187474f, 0.293892626146237, 0.f);
        glEnd();

        // Vertical lines
        glBegin(GL_LINES);
        glColor3f(1.f, 1.f, 1.f); // WHITE
        glVertex3f(0.0f, -480.0f, 0.0f);
        glVertex3f(0.0f, 480.0f, 0.0f);
        glEnd();

        glBegin(GL_LINES);
        glColor3f(1.f, 1.f, 1.f); // WHITE
        glVertex3f(-640.0f, 0.0f, 0.0f);
        glVertex3f(640.0f, 0.0f, 0.0f);
        glEnd();

        // glVertex3f(-0.6f, -0.4f, 0.f);
        // glColor3f(0.f, 0.f, 1.f);
        // // glColor3f(0.f, 1.f, 0.f);
        // glVertex3f(0.6f, -0.4f, 0.f);
        // glColor3f(0.f, 0.f, 1.f);
        // // glColor3f(1.f, 0.f, 0.f);
        // glVertex3f(0.f, 0.6f, 0.f);
        glEnd();
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}