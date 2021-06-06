#include <iostream>
#include "GLFW/glfw3.h"
#include "window.h"
#include "client.h"


int main()
{
	CoreNative::Window& window = CoreNative::Window::getInstanse();

#if 0
	Net::Client client(8888, "127.0.0.1");
	client.connect();
#endif

	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		glfwPollEvents();
		glfwSwapBuffers(window.getGLFWwindow());
	}


	return 0;
}