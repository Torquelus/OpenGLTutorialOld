#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

// Create struct for shader program source
struct ShaderProgramSource {
	std::string VertexSource;
	std::string FragmentSource;
};

// Parse the shader code
static ShaderProgramSource ParseShader(const std::string& filepath){
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
static unsigned int CompileShader(unsigned int type, const std::string& source) {
	// Create shader object of type type and return id
	unsigned int id = glCreateShader(type);

	// Turn source code to c string
	const char* src = source.c_str();

	// Set source code of shader pointed to by id, shader is 1 string, source code, nullptr to signify the string is null terminated
	glShaderSource(id, 1, &src, nullptr);

	// Compile our shader
	glCompileShader(id);

	// Check if compilation was successful
	int result;
	glGetShaderiv(id, GL_COMPILE_STATUS, &result);

	// Returns false if did not compile successfully
	if (result == GL_FALSE) {
		// Length of error message
		int length;

		// Gets error message length from shader
		glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);

		// Allocate space for message
		char* message = (char*)alloca(length * sizeof(char));

		// Get info log
		glGetShaderInfoLog(id, length, &length, message);

		// Print info
		std::cerr << "Failed to compile shader!" << std::endl;
		std::cerr << message << std::endl;

		// Delete shader and return 0
		glDeleteShader(id);
		return 0;
	}

	// Return shader id
	return id;
}

// Function to create shader from source shader code passed in as strings
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
	// Create program object and save reference
	unsigned int program = glCreateProgram();

	// Create and compile both shaders returning their id
	unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
	unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

	// Link the shaders to the same program
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	// Link and validate the program (link is like bind, validate checks the code and makes sure it works)
	glLinkProgram(program);
	glValidateProgram(program);

	// Delete our shaders after they have been linked
	glDeleteShader(vs);
	glDeleteShader(fs);

	// Return our program pointer
	return program;
}

int main(void)
{
	GLFWwindow* window;

	/* Initialize the library */
	if (!glfwInit()){
		std::cerr << "GLFW failed to initialise!" << std::endl;
		return -1;
	}

	/* Create a windowed mode window and its OpenGL context */
	window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	/* Make the window's context current */
	glfwMakeContextCurrent(window);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
		return -1;
	}

	// Print openGL version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Triangle vertices
	float positions[6] = {
		-0.5f, -0.5f,
		 0.0f,  0.5f,
		 0.5f, -0.5f
	};

	// Our buffer ID
	unsigned int buffer;

	// Generate 1 buffer with our buffer ID
	glGenBuffers(1, &buffer);

	// Bind our buffer
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// Create vertex position attribute with index 0 (only attribute), 2 floats, without normalising, with size between verteces being 2*floatsize, since there are no other attributes, and the attribute starts at 0
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0);

	// Enable vertex position attribute
	glEnableVertexAttribArray(0);

	// Fill our buffer with data
	glBufferData(GL_ARRAY_BUFFER, 6 * sizeof(float), positions, GL_STATIC_DRAW);

	// Generate shader source code from file
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	// Create shader from source code
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	glUseProgram(shader);
	
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		glClear(GL_COLOR_BUFFER_BIT);

		// Call draw on currently bound buffer
		glDrawArrays(GL_TRIANGLES, 0, 3);

		/* Swap front and back buffers */
		glfwSwapBuffers(window);

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete program
	glDeleteProgram(shader);

	glfwTerminate();
	return 0;
}
