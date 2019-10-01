#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

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

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
		return -1;
	}

	// Print openGL version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Triangle vertices
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// Our buffer ID
	unsigned int buffer;

	// Generate 1 buffer with our buffer ID
	glGenBuffers(1, &buffer);

	// Bind our buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Create vertex position attribute with index 0 (only attribute), 2 floats, without normalising, with size between verteces being 2*floatsize, since there are no other attributes, and the attribute starts at 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Enable vertex position attribute
	glEnableVertexAttribArray(0);

	// Fill our buffer with data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// Shader Time

	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Call draw on currently bound buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
