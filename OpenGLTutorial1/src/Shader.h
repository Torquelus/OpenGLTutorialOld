#pragma once

#include <string>
#include <unordered_map>

#include "glm/glm.hpp"

// Create struct for shader program source
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};


// Shader class
class Shader {
private:
	unsigned int m_RendererID;
	std::string m_FilePath;
	std::unordered_map<std::string, int> m_UniformLocationCache;
public:
	Shader(const std::string& filepath);	// Constructor
	~Shader();								// Destructor

	void Bind() const;						// Bind shader
	void Unbind() const;					// Unbind shader

	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);					// Set 4 float uniform
	void SetUniformMat4(const std::string& name, const glm::mat4& matrix);
private:
	ShaderProgramSource ParseShader(const std::string& filepath);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
	int GetUniformLocation(const std::string& name);
};