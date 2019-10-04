#include "VertexBuffer.h"

#include "Renderer.h"

// Constructor
VertexBuffer::VertexBuffer(const void* data, unsigned int size) {

	GLCall(glGenBuffers(1, &m_RendererID));								// Generate 1 buffer with our buffer ID
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));				// Bind our buffer
	GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));	// Fill our buffer with data

}

// Destructor
VertexBuffer::~VertexBuffer() {
	GLCall(glDeleteBuffers(1, &m_RendererID));
}

// Bind buffer
void VertexBuffer::Bind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID));
}

// Unbind buffer
void VertexBuffer::Unbind() const {
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
}