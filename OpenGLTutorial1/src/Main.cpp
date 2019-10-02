#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

#define ASSERT(x) if (!(x)) __debugbreak();
#define GLCall(x) GLClearError();\
	x;\
	ASSERT(GLLogCall(#x, __FILE__, __LINE__))

// Clear all OpenGL errors
static void GLClearError() {
	while (glGetError() != GL_NO_ERROR);
}

// Print all errors
static bool GLLogCall(const char* function, const char* file, int line) {
	while (GLenum error = glGetError()) {
		std::cerr << "OpenGL error: " << "0x" << std::hex << error << "\nFunction with error: " <<
			function << " in " << file << ":" << line << std::endl;
		return false;
	}
	return true;
}

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
static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader) {
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

	// Fix framerate
	glfwSwapInterval(1);

	// Initialise GLEW
	if (glewInit() != GLEW_OK) {
		std::cerr << "GLEW failed to initialise!" << std::endl;
		return -1;
	}

	// Print openGL version
	std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

	// Triangle vertices
	float positions[] = {
		-0.5f, -0.5f, //0
		 0.5f, -0.5f, //1
		 0.5f,  0.5f, //2
		-0.5f,  0.5f, //3
	};

	// Index buffer
	unsigned int indices[] = {
		0, 1, 2, // Triangle 1
		2, 3, 0  // Triangle 2
	};

	// CREATING A BUFFER FOR THE VERTICES
	// Our buffer ID
	unsigned int buffer;

	// Generate 1 buffer with our buffer ID
	GLCall(glGenBuffers(1, &buffer));

	// Bind our buffer
	GLCall(glBindBuffer(GL_ARRAY_BUFFER, buffer));

	// Create vertex position attribute with index 0 (only attribute), 2 floats, without normalising, with size between verteces being 2*floatsize, since there are no other attributes, and the attribute starts at 0
	GLCall(glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, 0));

	// Enable vertex position attribute
	GLCall(glEnableVertexAttribArray(0));

	// Fill our buffer with data
	GLCall(glBufferData(GL_ARRAY_BUFFER, 2 * 6 * sizeof(float), positions, GL_STATIC_DRAW));


	// CREATING A BUFFER FOR THE INDICES
	// Our index buffer ID
	unsigned int ibo;

	// Generate 1 index buffer with our buffer ID
	GLCall(glGenBuffers(1, &ibo));

	// Bind our buffer
	GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo));

	// Fill buffer with data
	GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW));

	// Generate shader source code from file
	ShaderProgramSource source = ParseShader("res/shaders/Basic.shader");

	// Create shader from source code
	unsigned int shader = CreateShader(source.VertexSource, source.FragmentSource);
	GLCall(glUseProgram(shader));

	// Creat colour uniform at location, assert that the location is found
	GLCall(int location = glGetUniformLocation(shader, "u_Color"));
	ASSERT(location != -1);
	GLCall(glUniform4f(location, 0.2f, 0.3f, 0.8f, 1.0f));
	
	// Red colour channel
	float r = 0.0f;
	float increment = 0.05f;

	// MAIN LOOP
	/* Loop until the user closes the window */
	while (!glfwWindowShouldClose(window))
	{
		/* Render here */
		GLCall(glClear(GL_COLOR_BUFFER_BIT));

		// Set Colour
		GLCall(glUniform4f(location, r, 0.3f, 0.8f, 1.0f));

		// Call draw on currently bound buffer
		GLCall(glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr));

		// Increment r
		if (r > 1.0f) {
			increment = -0.05f;
		}
		else if (r < 0.0f) {
			increment = 0.05f;
		}

		r += increment;

		/* Swap front and back buffers */
		GLCall(glfwSwapBuffers(window));

		/* Poll for and process events */
		glfwPollEvents();
	}

	// Delete program
	GLCall(glDeleteProgram(shader));

	glfwTerminate();
	return 0;
}
