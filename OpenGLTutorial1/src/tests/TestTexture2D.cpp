#include "TestTexture2D.h"

#include "Renderer.h"
#include "imgui/imgui.h"
#include <iostream>

namespace test {

	// Constructor
	TestTexture2D::TestTexture2D() {
	}

	// Destructor
	TestTexture2D::~TestTexture2D() {
		
	}

	void TestTexture2D::OnUpdate(float deltaTime){

	}

	// Render Objects
	void TestTexture2D::OnRender(){
		// Set clear colour and clear screen
		GLCall(glClearColor(0.0f, 0.0f, 0.0f, 1.0f));
		GLCall(glClear(GL_COLOR_BUFFER_BIT));
	}

	// Render GUI
	void TestTexture2D::OnImGuiRender(){
		
	}

}