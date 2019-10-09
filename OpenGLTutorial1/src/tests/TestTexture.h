#pragma once

#include "Test.h"
#include "Texture.h"
#include "Shader.h"
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "IndexBuffer.h"

namespace test {

	class TestTexture : public Test {
	private:
		Texture *texture;
		Shader *shader;
		VertexBuffer *vb;
		IndexBuffer* ib;
		VertexBufferLayout *layout;

		float width, height;
	public:
		TestTexture();							// Constructor
		~TestTexture();							// Destructor

		void OnUpdate(float deltaTime) override;	// Each frame (or deltaTime)
		void OnRender() override;					// Render objects
		void OnImGuiRender() override;				// Render GUI
	};

}