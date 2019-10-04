#include "IndexBuffer.h"

#include "Renderer.h"

// Constructor
IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) {
	ASSERT(sizeof(unsigned int) == sizeof(GLuint));

	GLCall(glGenBuffers(1, &m_RendererID));								// Generate 1 buffer with our buffer ID
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));				// Bind our buffer
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, GL_STATIC_DRAW));	// Fill our buffer with data

	m_Count = count;
}

// Destructor
IndexBuffer::~IndexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

// Bind buffer
void IndexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID));
}

// Unbind buffer
void IndexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}