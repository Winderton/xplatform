#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include "window.h"

#include "glm/glm.hpp"
#include "glm/vec2.hpp"



const std::string readFromFile(const char* pathToFile)
{
	std::string content;
	
	std::ifstream fileStream(pathToFile, std::ios::in);

	std::string line = "";
	while (!fileStream.eof()) {
		std::getline(fileStream, line);
		content.append(line + "\n");
	}

	fileStream.close();
	return content;
}

GLuint execute(const std::string& vert, const std::string& frag)
{
	GLuint program = glCreateProgram();

	GLuint id = glCreateShader(GL_VERTEX_SHADER);
	std::string result = readFromFile(vert.c_str());
	const char* src = result.c_str();
	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);
	GLuint vs = id;
	

	GLuint id2 = glCreateShader(GL_FRAGMENT_SHADER);
	std::string result2 = readFromFile(frag.c_str());
	const char* src2 = result2.c_str();
	glShaderSource(id2, 1, &src2, nullptr);
	glCompileShader(id2);
	GLuint fs = id2;


	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);
	glValidateProgram(program);
	

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main(void)
{
	CoreNative::Window& window = CoreNative::Window::getInstanse();

	//glm::vec2 v1(-0.5f, -0.5f);
	//glm::vec2 v2(0.0f, 0.5f);
	//glm::vec2 v3(0.5f, -0.5f);


	

	unsigned buffer;

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);
	


	GLuint shader = execute("src/basicVertex.shader", "src/basicFragment.shader");
	glUseProgram(shader);

	while (!glfwWindowShouldClose(window.getGLFWwindow()))
	{
		glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), window.dots, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);
		glClear(GL_COLOR_BUFFER_BIT);

		glDrawArrays(GL_TRIANGLES, 0, 3);
		
		glfwSwapBuffers(window.getGLFWwindow());

 		glfwPollEvents();

	}

	glfwTerminate();
	glDeleteProgram(shader);

	return 0;
}