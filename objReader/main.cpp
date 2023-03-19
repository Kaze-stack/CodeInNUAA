#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"

#include "myLibrary.h"

int main(int argc, char *argv[])
{
	WindowHint();
	GLFWwindow *window=glfwCreateWindow(800,800,"GL_Test",NULL,NULL);
	glfwMakeContextCurrent(window);
    glewExperimental=GL_TRUE;
	if (glewInit()!=GLEW_OK) exit(FAIL);
	glfwSwapInterval(1);

	glfwSetWindowSizeCallback(window,window_Reshape);
	init(window);

	while (!glfwWindowShouldClose(window))
    {
		display(window,glfwGetTime());
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(SUCCESS);
	return 0;
}
#pragma clang diagnostic pop
