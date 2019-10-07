#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()){
		std::cerr << "GLFW failed to initialise!" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	// Set up minimum version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	{

		// Fix framerate
		glfwSwapInterval(1);

		// Initialise GLEW
		if (glewInit() != GLEW_OK) {
			std::cerr << "GLEW failed to initialise!" << std::endl;
			return -1;
		}

		// Print openGL version
		std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

		// Triangle vertices
		float positions[] = {
			-0.5f, -0.5f, //0
			 0.5f, -0.5f, //1
			 0.5f,  0.5f, //2
			-0.5f,  0.5f, //3
		};

		// Index buffer
		unsigned int indices[] = {
			0, 1, 2, // Triangle 1
			2, 3, 0  // Triangle 2
		};

		// CREATE VERTEX ARRAY OBJECT
		VertexArray va;
		VertexBuffer vb(positions, 4 * 2 * sizeof(float));
		VertexBufferLayout layout;
		layout.Push<float>(2);
		va.AddBuffer(vb, layout);

		IndexBuffer ib(indices, 6);

		va.Bind();

		// Create Shader
		Shader shader("res/shaders/Basic.shader");
		shader.Bind();
		shader.SetUniform4f("u_Color", 0.2f, 0.3f, 0.8f, 1.0f);

		// Red colour channel
		float r = 0.0f;
		float increment = 0.05f;

		// MAIN LOOP
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window))
		{
			/* Render here */
			GLCall(glClear(GL_COLOR_BUFFER_BIT));
			// Set Colour
			shader.SetUniform4f("u_Color", r, 0.3f, 0.8f, 1.0f);
			// Call draw on currently bound buffer
			GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));
			// Increment r to animate the red colour
			if (r > 1.0f) {
				increment = -0.05f;
			}
			else if (r < 0.0f) {
				increment = 0.05f;
			}
			r += increment;
			/* Swap front and back buffers */
			GLCall(glfwSwapBuffers(window));
			/* Poll for and process events */
			glfwPollEvents();
		}

	}

	// Cleanup
	glfwTerminate();
	return 0;
}
