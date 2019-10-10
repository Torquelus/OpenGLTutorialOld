#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "Shader.h"
#include "Texture.h"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

#include "imgui/imgui.h"
#include "imgui/imgui_impl_glfw.h"
#include "imgui/imgui_impl_opengl3.h"

#include "tests/TestClearColour.h"
#include "tests/TestTexture2D.h"
#include "tests/TestMenu.h"

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()){
		std::cerr << "GLFW failed to initialise!" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(1280, 720, "Hello World", NULL, NULL);
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

	// Fix framerate
	glfwSwapInterval(1);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
		return -1;
	}

	// Print openGL version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	{

		// enable blending
		GLCall(glEnable(GL_BLEND));
		GLCall(glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA));

		// Create renderer
		Renderer renderer;

		// INITIALISE IMGUI
		ImGui::CreateContext();
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init();
		ImGui::StyleColorsDark();

		// Initialise to main menu
		test::Test *currentTest = nullptr;
		test::TestMenu* testMenu = new test::TestMenu(currentTest);
		currentTest = testMenu;

		testMenu->RegisterTest<test::TestClearColour>("Clear Colour");

		// MAIN LOOP
		/* Loop until the user closes the window */
		while (!glfwWindowShouldClose(window)){

			// Clear renderer
			renderer.Clear();
			
			// Start IMGUI frame
			ImGui_ImplOpenGL3_NewFrame();
			ImGui_ImplGlfw_NewFrame();
			ImGui::NewFrame();

			// Render current test
			if (currentTest) {
				currentTest->OnUpdate(0.0f);
				currentTest->OnRender();
				ImGui::Begin("Test");
				if (currentTest != testMenu && ImGui::Button("Back")) {
					delete currentTest;
					currentTest = testMenu;
				}
				currentTest->OnImGuiRender();
				ImGui::End();
			}
			
			// Render ImGUI
			ImGui::Render();
			ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

			/* Swap front and back buffers */
			glfwSwapBuffers(window);
			/* Poll for and process events */
			glfwPollEvents();
		}

		// Clean up test
		delete currentTest;
		if (currentTest != testMenu) {
			delete testMenu;
		}

	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	glfwTerminate();
	return 0;
}
