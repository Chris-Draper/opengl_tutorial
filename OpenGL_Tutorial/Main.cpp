// this makes it so that a console doesn't show up behind the render window
// there are other ways 
#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// register callback function for resizing window
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

/* The shader that draws the vertex points. You have to cast a 3D vertex to a 4D vertex
because drawing the triangle only requires 3 dimensions but the default vertex is in
4D. The 4th dimension "w" is used for perspective division which I will learn later.
Shaders are written in a coding language called "GLSL" = OpenGL Shading Language*/
const char *vertexShaderSource = "#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main()\n"
"{\n"
"   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
"}\0";

/* The shader that colors in the pixels that go inbetween the different shapes that
have been defined in the graphics pipeline. This is also written in "GLSL". This shader
gives all of the pixels their color. It's worth noting that a fragment is a pixel after
the vector shapes have been rasterized.*/
const char *fragmentShaderSource = "#version 330 core\n"
"out vec4 FragColor;\n"
"void main()\n"
"{\n"
"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\n\0";

int main() 
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	//need to do this on MAC OS ONLY
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	
	// create the opengl window pointer
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// set opengl's focus to the current window
	glfwMakeContextCurrent(window);

	// register the callback function for resizing the opengl window
	// this gets called when the window is first open with default width and height
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

	// setup glad to manage function pointers in opengl
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// -------------
	// VERTEX SHADER
	// -------------

	// create the int id for the vertex shader and state type of shader
	unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
	// associate shader id with the GLSL that creates the shader
	// 1 is the number of strings passed to the shader as source code
	glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
	// compile the const C string to turn code into OpenGL shader
	glCompileShader(vertexShader);

	// after you build the shader you have to check for shading compile errors
	int success;
	// buffer to store text error if shader fails
	char infoLog[512];
	// make the first call to the shader
	glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
	// check the success variable
	if (!success)
	{
		// get why the shader failed and print that message to console
		glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// ---------------
	// FRAGMENT SHADER
	// ---------------

	// give the shader an id and define as type fragment shader
	unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
	// set the source code that defines how fragment shader functions
	glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
	// compile the source code
	glCompileShader(fragmentShader);
	// reuse success and infoLog char buffer from testing vertex shader
	glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
	// check if shader compiled and print error message if it failed
	if (!success)
	{
		glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// --------------
	// SHADER PROGRAM
	// --------------

	// A shader program links the different shaders together
	// Points -> Vertex Shader -> Default Geometry Shader -> Fragment Shader -> Screen Render

	unsigned int shaderProgram = glCreateProgram();
	// first attach vertex shader then fragment shader
	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);
	// second link the two shaders together
	glLinkProgram(shaderProgram);

	// test the linked up shader program
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
	}

	// now that a complete shader program is made, delete the individual shaders
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	// ----------------------------------------------------
	// Gen VAO, Gen VBO, Gen Attributes, Store VBO into VAO
	// VAO = Vertex Attribute Object
	// VBO = Vertex Buffer Object
	// ----------------------------------------------------

	// setup the data for the triangle's vertices
	float vertices[] = {
		 0.5f,  0.5f, 0.0f,  // top right
		 0.5f, -0.5f, 0.0f,  // bottom right
		-0.5f, -0.5f, 0.0f,  // bottom left
		-0.5f,  0.5f, 0.0f   // top left 
	};
	// get id of the vertex buffer object
	unsigned int VBO, VAO;
	// generate the VAO and the VBO
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	// bind the VAO first, then bind VBO, then set vertex attributes
	glBindVertexArray(VAO);
	// set the type of the buffer - aka array buffer
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// fill the buffer with the vertices' data and say static draw is infrequent update
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// set the vertex attribute pointers and enable them
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	// unbind the VBO since it is already saved in the VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// unbind the VAO so other VAO calls won't effect this specific VAO
	glBindVertexArray(0);	

	// -----------
	// RENDER LOOP
	// -----------

	// the render loop that tells opengl to stay open unless told to exit
	while (!glfwWindowShouldClose(window))
	{
		// check for user input every render loop
		processInput(window);

		// window rendering commands will go here
		// set the color you want to clear the entire window with
		// this is a state setting function
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		// clear the window with the previously set color
		// this is a state using function
		glClear(GL_COLOR_BUFFER_BIT);

		// DRAW THE TRIANGLE :D
		// tell GL to use the new shader program whenever render is called
		glUseProgram(shaderProgram);
		// bind VAO every time you render to keep things organized
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);

		// swap out colors in the buffer based on updated user input
		glfwSwapBuffers(window);
		// check for any user input
		glfwPollEvents();
	}

	// delete all of glfw's resources used to render after done rendering
	glfwTerminate();
	return 0;
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

// check if the user pressed ESC and set close window if they did
void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}