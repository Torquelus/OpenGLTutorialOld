#include "Renderer.h"

#include <iostream>

// Clear all OpenGL errors
void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// Print all errors
bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cerr << "OpenGL error: " << "0x" << std::hex << error << "\nFunction with error: " <<
			function << " in " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}