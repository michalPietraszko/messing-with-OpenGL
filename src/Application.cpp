#define GLEW_STATIC
#include <GL/glew.h> // Cross - platform proxy between OPENGL functions defined in GPU drivers and user interface
#include <GLFW/glfw3.h> // Providde a simple API for creating windows, context and surfaces, receiving inputs and events
// GLFW is written in C
#include <iostream>

#include <fstream>
#include <string>
#include <sstream>
#include <cmath>
#include <utility>

#include "Renderer.hpp"
#include "VertexBuffer.hpp"
#include "IndexBuffer.hpp"
#include "VertexArray.hpp"

namespace
{	
	template<int N>
	float changeChannelGetIncrement(const float(&rgb)[N], int& currentIndex)
	{
		static bool isAdding = true;
		if((rgb[currentIndex] >= 1 and isAdding) or (rgb[currentIndex] <= 0 and not isAdding))
		{
			currentIndex++;
			if(currentIndex == N)
			{
				currentIndex = 0;
				isAdding = not isAdding;	
			}
		}
		return isAdding ? 0.005f : -0.005f;
	}
};

struct ShaderProgramSource
{
	std::string VertexSource;
	std::string FragmentSource;
};

static ShaderProgramSource ParseShader(const std::string& filepath)
{
	std::ifstream stream(filepath);

	enum class ShaderType
	{
		NONE = -1,
		VERTEX = 0,
		FRAGMENT = 1
	};

	std::string line;
	std::stringstream ss[2];
	ShaderType type = ShaderType::NONE;

	while (getline(stream, line))
	{
		if (line.find("#shader") != std::string::npos)
		{
			if (line.find("vertex") != std::string::npos)
			{
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos)
			{
				type = ShaderType::FRAGMENT;
			}
		}
		else
		{
			ss[static_cast<int>(type)] << line << '\n';
		}
	}
	return {ss[0].str(), ss[1].str()};
}

static unsigned int CompileShader(unsigned int type, const std::string &source)
{
	unsigned int id = glCreateShader(type);
	const char *src = source.c_str();

	glShaderSource(id, 1, &src, nullptr);
	glCompileShader(id);

	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	if (result == GL_FALSE)
	{
		int length;
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		char* message = new char[length];
		//char *message = (char *)alloca(length * sizeof(char));
		glGetShaderInfoLog(id, length, &length, message);

		std::cout << "Failed to compile " << (type == GL_VERTEX_SHADER ? "vertex " : "fragment")
				  << " shader!" << std::endl;
		std::cout << message << std::endl;

		glDeleteShader(id);
		delete [] message;
		return 0;
	}
	return id;
}

static unsigned int CreateShader(const std::string &vertexShader, const std::string &fragmentShader)
{
	unsigned int program = glCreateProgram();
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);
	glValidateProgram(program);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return program;
}


int main()
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit())
		return -1;

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);//ver 3.3 
	// core profile doesn't make a default vao 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);

	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);
	//vsync
	glfwSwapInterval(1);

	float positions[] = 
	{
		-0.5f, -0.5f,
		 0.5f, -0.5f,
		 0.5f, 0.5f,
		-0.5f, 0.5f
	};

	unsigned int indices[] = 
	{
		0, 1, 2,
		2, 3, 0
	};

	glewInit();

	VertexBuffer vertexBuffer(positions, 4 * 2 * sizeof(float));

	VertexArray vertexArray;
	VertexBufferLayout layout;

	layout.push<float>(2); // 2 as x and y coordinates
	vertexArray.addBuffer(vertexBuffer, layout);

	IndexBuffer indexBuffer(indices, 6);

	const ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);

	// bind shader
	GLCall(glUseProgram(shader));

	// look up id of uniform from shader by name
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));

	ASSERT(location != -1)
	// set the uniform 
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));

	glBindVertexArray(0);
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	float rgb[] = {0, 0, 0};
	int currentIndex{0};

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
		// bind everything again
		GLCall(glUseProgram(shader));
		GLCall(glUniform4f(location, fabs(rgb[0]), fabs(rgb[1]), fabs(rgb[2]) ,1.0f));

		vertexArray.bind();
		indexBuffer.bind();
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		rgb[currentIndex] += changeChannelGetIncrement(rgb, currentIndex);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glDeleteProgram(shader);
	glfwTerminate();

	return 0;
}