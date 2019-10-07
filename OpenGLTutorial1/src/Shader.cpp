#include "Shader.h"

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#include "Renderer.h"

// Constructor
Shader::Shader(const std::string& filepath) {
	m_FilePath = filepath;
	m_RendererID = 0;

	// Generate shader source code from file
	ShaderProgramSource source = ParseShader(filepath);
	// Create shader from source code
	m_RendererID = CreateShader(source.VertexSource, source.FragmentSource);
}

// Destructor
Shader::~Shader() {
	GLCall(glDeleteProgram(m_RendererID));
}

// Bind shader
void Shader::Bind() const {
	GLCall(glUseProgram(m_RendererID));
}

// Unbind shader
void Shader::Unbind() const {
	GLCall(glUseProgram(0));
}

// Set 1 float uniform
void Shader::SetUniform1f(const std::string& name, float value) {
	GLCall(glUniform1f(GetUniformLocation(name), value));
}

// Set 4 float uniform
void Shader::SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3) {
	GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
}

// Get uniform location
unsigned int Shader::GetUniformLocation(const std::string& name) {
	if (m_UniformLocationCache.find(name) != m_UniformLocationCache.end()) {
		return m_UniformLocationCache[name];
	}

	GLCall(int location = glGetUniformLocation(m_RendererID, name.c_str()));
	if (location == -1) {
		std::cout << "Warning: uniform '" << name << "' does not exist!" << std::endl;
	}
	
	m_UniformLocationCache[name] = location;

	return location;
}

// Parse the shader code
ShaderProgramSource Shader::ParseShader(const std::string& filepath) {
	// Open file
	std::ifstream stream(filepath);

	// Create shaders enum, set to 0 and 1 for the array
	enum class ShaderType {
		NONE = -1, VERTEX = 0, FRAGMENT = 1
	};

	// Create line string
	std::string line;

	// Create stringstream
	std::stringstream ss[2];

	// Initialise type to none
	ShaderType type = ShaderType::NONE;

	// Loop through file
	while (getline(stream, line)) {
		// If found #shader in line
		if (line.find("#shader") != std::string::npos) {
			if (line.find("vertex") != std::string::npos) {
				// Set type to vertex
				type = ShaderType::VERTEX;
			}
			else if (line.find("fragment") != std::string::npos) {
				// Set mode to fragment
				type = ShaderType::FRAGMENT;
			}
		}
		else {
			// Add lines to appropriate stringstream
			ss[(int)type] << line << '\n';
		}
	}

	// Return source code in struct
	return { ss[0].str(), ss[1].str() };
}

// Compile a shader to be called in other program
unsigned int Shader::CompileShader(unsigned int type, const std::string& source) {
	// Create shader object of type type and return id
	GLCall(unsigned int id = glCreateShader(type));

	// Turn source code to c string
	const char* src = source.c_str();

	// Set source code of shader pointed to by id, shader is 1 string, source code, nullptr to signify the string is null terminated
	GLCall(glShaderSource(id, 1, &src, nullptr));

	// Compile our shader
	GLCall(glCompileShader(id));

	// Check if compilation was successful
	int result;
	GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result));

	// Returns false if did not compile successfully
	if (result == GL_FALSE) {
		// Length of error message
		int length;

		// Gets error message length from shader
		GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));

		// Allocate space for message
		char* message = (char*)alloca(length * sizeof(char));

		// Get info log
		GLCall(glGetShaderInfoLog(id, length, &length, message));

		// Print info
		std::cerr << "Failed to compile shader!" << std::endl;
		std::cerr << message << std::endl;

		// Delete shader and return 0
		GLCall(glDeleteShader(id));
		return 0;
	}

	// Return shader id
	return id;
}

// Function to create shader from source shader code passed in as strings
unsigned int Shader::CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// Create program object and save reference
	GLCall(unsigned int program = glCreateProgram());

	// Create and compile both shaders returning their id
	GLCall(unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader));
	GLCall(unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader));

	// Link the shaders to the same program
	GLCall(glAttachShader(program, vs));
	GLCall(glAttachShader(program, fs));

	// Link and validate the program (link is like bind, validate checks the code and makes sure it works)
	GLCall(glLinkProgram(program));
	GLCall(glValidateProgram(program));

	// Delete our shaders after they have been linked
	GLCall(glDeleteShader(vs));
	GLCall(glDeleteShader(fs));

	// Return our program pointer
	return program;
}