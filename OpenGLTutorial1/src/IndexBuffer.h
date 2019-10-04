#pragma once

// Class that handles vertex buffers
class IndexBuffer {
private:
	unsigned int m_RendererID;		// ID for a generated buffer
	unsigned int m_Count;			// Number of indices
public:
	IndexBuffer(const unsigned int* data, unsigned int count);	// Constructor
	~IndexBuffer();					// Destructor
	void Bind() const;				// Bind bufer
	void Unbind() const;			// Unbind buffer

	inline unsigned int GetCount() const { return m_Count;  }
};