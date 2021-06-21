#include <iostream>
#include "GLFW/glfw3.h"
#include "window.h"
#include "client.h"
#include "glm/glm.hpp"
#include "glm/vec3.hpp"
#include "glm/gtx/string_cast.hpp"





int main()
{
	CoreNative::Window& window = CoreNative::Window::getInstanse();
	glm::vec3 vec3(1.1f, 1.2f, 1.3f);
	std::cout << glm::to_string(vec3) << std::endl;
	
	
#if 0
	Net::Client client(8888, "127.0.0.1");
	client.connect();


	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		glfwPollEvents();
		glfwSwapBuffers(window.getGLFWwindow());
	}
#endif

	return 0;
}