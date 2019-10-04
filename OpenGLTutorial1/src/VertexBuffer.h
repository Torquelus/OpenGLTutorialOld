#pragma once

// Class that handles vertex buffers
class VertexBuffer {
private:
	unsigned int m_RendererID;		// ID for a generated buffer
public:
	VertexBuffer(const void* data, unsigned int size);	// Constructor
	~VertexBuffer();				// Destructor
	void Bind() const;				// Bind bufer
	void Unbind() const;			// Unbind buffer
};