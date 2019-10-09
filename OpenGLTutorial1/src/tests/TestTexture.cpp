#include "TestTexture.h"

#include "Renderer.h"
#include "imgui/imgui.h"

namespace test {

	// Constructor
	TestTexture::TestTexture() {

		// Width and Height of texture
		width = 100.0f;
		height = 100.0f;

		// Create vertex buffer
		float positions[] = {
			(-width / 2),	(-height / 2),	0.0f, 0.0f,		// 0
			(width / 2),	(-height / 2),	0.0f, 1.0f,		// 1
			(width / 2),	(height / 2),	1.0f, 1.0f,		// 2
			(-width / 2),	(height / 2),	1.0f, 0.0f		// 3
		};
		vb = new VertexBuffer(positions, 4 * 4 * sizeof(float));
		vb->Bind();

		// Create index buffer
		unsigned int indices[] = {
			0, 1, 2,
			2, 3, 0
		};
		ib = new IndexBuffer(indices, 6);
		ib->Bind();

		// Create shader
		shader = new Shader("res/shaders/Basic.shader");
		shader->Bind();

		// Create texture
		texture = new Texture("res/textures/n64.png");
		texture->Bind();
	}

	// Destructor
	TestTexture::~TestTexture() {

	}

	void TestTexture::OnUpdate(float deltaTime) {

	}

	// Render Objects
	void TestTexture::OnRender() {
		// Bind Texture		
		texture->Bind();
	}

	// Render GUI
	void TestTexture::OnImGuiRender() {
		// Set height and width
		ImGui::SliderFloat("Width", &width, 0.0f, 1280.0f);
		ImGui::SliderFloat("Height", &height, 0.0f, 720.0f);
	}

}