#pragma once

#include "Test.h"

#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
#include "Texture.h"

#include <memory>

namespace test {

	class TestTexture2D : public Test {
	private:
		std::unique_ptr<VertexArray> m_VAO;
		std::unique_ptr<VertexBuffer> m_VertexBuffer;
		std::unique_ptr<IndexBuffer> m_IndexBuffer;
		std::unique_ptr<Shader> m_Shader;
		std::unique_ptr<Texture> m_Texture;

		glm::mat4 m_Proj, m_View;
		glm::vec3 m_TranslationA, m_TranslationB;
	public:
		TestTexture2D();							// Constructor
		~TestTexture2D();							// Destructor

		void OnUpdate(float deltaTime) override;	// Each frame (or deltaTime)
		void OnRender() override;					// Render objects
		void OnImGuiRender() override;				// Render GUI
	};

}