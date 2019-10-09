#include "TestClearColour.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include <iostream>

namespace test {

	// Constructor
	TestClearColour::TestClearColour() : m_ClearColour{ 0.2f, 0.3f, 0.8f, 1.0f } {
	}

	// Destructor
	TestClearColour::~TestClearColour() {
		
	}

	void TestClearColour::OnUpdate(float deltaTime){

	}

	// Render Objects
	void TestClearColour::OnRender(){
		// Set clear colour and clear screen
		GLCall(glClearColor(m_ClearColour[0], m_ClearColour[1], m_ClearColour[2], m_ClearColour[3]));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	// Render GUI
	void TestClearColour::OnImGuiRender(){
		ImGui::ColorEdit4("Clear Colour", m_ClearColour);
	}

}