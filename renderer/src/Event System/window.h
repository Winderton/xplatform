#pragma once
#include <string>
#include <functional>
#include "event.h"

#define GLEW_STATIC

#include "GL/glew.h"
#include "GLFW/glfw3.h"


namespace CoreNative
{
	class Window final
	{
	public:
		float dots[6] = {
				-0.5f, -0.5f,
				0.0f, 0.5f,
				0.5f, -0.5f
		};
	private:
		std::string name;
		int width = 0;
		int height = 0;
		GLFWwindow* window = nullptr;
		std::function<void(Event&)> fnCallback;
	public:
		Window(const Window&) = delete;
		Window& operator=(const Window&) = delete;
		
	private:
		Window(const std::string& name, int width, int height);
	private:
		~Window();
	public:
		static Window& getInstanse() { static Window instanse("Window", 1280, 720); return instanse; }
	public:
		void init(const std::string& name, int width, int height);
		__forceinline GLFWwindow* getGLFWwindow() const { return window; }
		void setfnCallback(const std::function<void(Event&)>& fn) { fnCallback = fn; }
	public:
		static void mouseMovedCallback(GLFWwindow* window, double x, double y);
		static void mouseScrollCallback(GLFWwindow* window, double x, double y);
		static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
		static void keyCallback(GLFWwindow*, int key, int scancode, int action, int mods);
		static void windowResizeCallback(GLFWwindow* window, int width, int height);
		static void windowCloseCallback(GLFWwindow* window);
	};
}