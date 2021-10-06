#include <iostream>
#include "window.h"
#include "mouseevent.h"
#include "keyboardevent.h"
#include "windowevent.h"




#include <sstream>

namespace CoreNative
{
	Window::Window(const std::string& name, int width, int height)
	{
		init(name, width, height);

		setfnCallback([this](Event& e)
			{
				if (e.getType() == Event::EventType::WINDOW_CLOSED_EVENT)
				{
					glfwSetWindowShouldClose(window, GL_TRUE);
				}
				std::cout << e.format() << std::endl;
			});

	}

	void Window::init(const std::string& name, int width, int height)
	{


		this->name = name;
		this->width = width;
		this->height = height;


		if (!glfwInit())
		{
			__asm {
				int 3
			}
		}

		window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);


		

		if (!window)
		{
			__asm {
				int 3
			}
		}


		glfwMakeContextCurrent(window);

		if (glewInit() != GLEW_OK)
			std::cout << "GLEW failed..." << std::endl;

		glfwSetWindowUserPointer(window, this);
		glfwSetCursorPosCallback(window, mouseMovedCallback);
		glfwSetScrollCallback(window, mouseScrollCallback);
		glfwSetMouseButtonCallback(window, mouseButtonCallback);
		glfwSetKeyCallback(window, keyCallback);
		glfwSetWindowSizeCallback(window, windowResizeCallback);
		glfwSetWindowCloseCallback(window, windowCloseCallback);
	}



	void Window::mouseMovedCallback(GLFWwindow* window, double x, double y)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		MouseMovedEvent e(x, y);
		handle.fnCallback(e);
	}


	void Window::mouseScrollCallback(GLFWwindow* window, double x, double y)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		MouseScrolledEvent e(y);
		handle.fnCallback(e);
	}


	void Window::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		if (action == GLFW_PRESS)
		{
			MouseButtonPressed e(button);
			handle.fnCallback(e);
		}
		else
		{
			MouseButtonReleased e(button);
			handle.fnCallback(e);
		}
	}



	void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);

		if (action == GLFW_PRESS)
		{
			if (key == GLFW_KEY_SPACE)
			{
				handle.dots[0] = -1.0f;
				handle.dots[1] = -1.0f;
				handle.dots[2] = 0.0f;
				handle.dots[3] = 1.0f;
				handle.dots[4] = 1.0f;
				handle.dots[5] = -1.0f;
			}
			KeyPressedEvent e(key);
			handle.fnCallback(e);
		}
		else if (action == GLFW_RELEASE)
		{
			if (key == GLFW_KEY_SPACE)
			{
				handle.dots[0] = -0.5f;
				handle.dots[1] = -0.5f;
				handle.dots[2] = 0.0f;
				handle.dots[3] = 0.5f;
				handle.dots[4] = 0.5f;
				handle.dots[5] = -0.5f;
			}
			KeyReleasedEvent e(key);
			handle.fnCallback(e);
		}
	}




	void Window::windowResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		WindowResizedEvent e(width, height);
		handle.width = width;
		handle.height = height;
		handle.fnCallback(e);

	}


	void Window::windowCloseCallback(GLFWwindow* window)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		WindowClosedEvent e;
		handle.fnCallback(e);
	}




	Window::~Window()
	{
		glfwWindowShouldClose(window);
	}

}