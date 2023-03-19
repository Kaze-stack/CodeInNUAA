#ifndef myLibrary_h
#define myLibrary_h

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL2/SOIL2.h>

#include <iostream>
#include <fstream>
#include <string>
#include <stack>
#include <cmath>
using namespace std;

#define SUCCESS 0
#define FAIL 1

float toRad(float degree);
string readShaderSource(const char *filePath);
GLuint createShaderProgram();
void WindowHint();
GLuint loadTexture(const char *filePath);

void SetupVertices();

void init(GLFWwindow *window);
void window_Reshape(GLFWwindow *window,int n_width,int n_height);
void display(GLFWwindow *window,double currentTime);
#endif
