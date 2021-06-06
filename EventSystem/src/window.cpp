#include <iostream>
#include "window.h"
#include "mouseevent.h"
#include "keyboardevent.h"
#include "windowevent.h"

#include <serialization.h>
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

	using namespace ObjectModel;

	void Window::init(const std::string& name, int width, int height)
	{
		
		std::vector<uint8_t> objectFromFile = Core::Util::load("WindowParams.abc");
		if (!objectFromFile.empty())
		{
			int16_t it = 0;
			ObjectModel::Object toPrintObject = ObjectModel::Object::unpack(objectFromFile, it);

			int16_t it2 = 0;
			width = Core::decode<int32_t>(toPrintObject.findPrimitiveByName("width").getData(), it2);

			int16_t it3 = 0;
			height = Core::decode<int32_t>(toPrintObject.findPrimitiveByName("height").getData(), it3);
		}
		else
		{
			this->name = name;
			this->width = width;
			this->height = height;
		}
		

		


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
			KeyPressedEvent e(key);
			handle.fnCallback(e);
		}
		else if (action == GLFW_RELEASE)
		{
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

		ObjectModel::Object storage("WindowParams");
		std::unique_ptr<ObjectModel::Primitive> pWidth = ObjectModel::Primitive::create("width", ObjectModel::Type::I32, width);
		std::unique_ptr<ObjectModel::Primitive> pHeight = ObjectModel::Primitive::create("height", ObjectModel::Type::I32, height);
		storage.addEntity(pWidth.get());
		storage.addEntity(pHeight.get());

		Core::Util::retriveNsave(&storage);
	}


	void Window::windowCloseCallback(GLFWwindow* window)
	{
		auto& handle = *(Window*)glfwGetWindowUserPointer(window);
		WindowClosedEvent e;
		handle.fnCallback(e);
	}


	void Window::store()
	{
		
	}

	void Window::load()
	{
		
	}


	Window::~Window()
	{
		glfwWindowShouldClose(window);
	}

}