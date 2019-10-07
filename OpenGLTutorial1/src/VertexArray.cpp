#include "VertexArray.h"

#include "VertexBufferLayout.h"
#include "Renderer.h"

// Constructor
VertexArray::VertexArray() {
	GLCall(glGenVertexArrays(1, &m_RendererID));				// Create VAO
}

// Destructor
VertexArray::~VertexArray() {
	GLCall(glDeleteVertexArrays(1, &m_RendererID));
}

// Add buffer to va
void VertexArray::AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout) {
	
	// Bind the buffer and vertex array
	vb.Bind();
	Bind();

	// Get all elements from layout and loop through them
	const auto& elements = layout.GetElements();
	unsigned int offset = 0;
	for (unsigned int i = 0; i < elements.size(); i++) {
		const auto& element = elements[i];
		// For each element enable attribut and define attrib pointer
		GLCall(glEnableVertexAttribArray(i));
		GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.GetStride(), (const void*) offset));
		offset += element.count * VertexBufferElement::GetSizeOfType(element.type);
	}
}

// Bind va
void VertexArray::Bind() const {
	GLCall(glBindVertexArray(m_RendererID));
}

// Unbind va
void VertexArray::Unbind() const {
	GLCall(glBindVertexArray(0));
}